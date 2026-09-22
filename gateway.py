
#Import important libraries

import json
import os
import anthropic
import requests

#picking sonnet for anthropic model
MODEL = "claude-sonnet-5"

# stores key in the terminal
client = anthropic.Anthropic()


light_is_on = False
url = "http://192.168.1.110"

def set_light(on):
    """Switch a desk ESP32 light. Later: publish an MQTT message."""
    value = 1 if on else 0
    website = f"{url}/light?on={value}"

    try:
        r = requests.get(website, timeout=3)
    except requests.RequestException:
        return "I couldn't reach the desk light."

    if r.ok:
        return r.text
    return f"The light refused that: {r.text}"


TOOLS = [
    {
        "name": "set_light",
        "description": (
            "Turn the desk light on or off. Use this whenever the user asks "
            "for the light to be switched, dimmed to off, or turned back on."
        ),
        "input_schema": {
            "type": "object",
            "properties": {
                "on": {
                    "type": "boolean",
                    "description": "true to turn the light on, false to turn it off",
                }
            },
            "required": ["on"],
        },
    }
]

SYSTEM = (
    "You are Porter, a small device in Bayram's apartment. "
    "You will eventually speak your replies out loud, so keep them to one "
    "short sentence. Never explain that you are using a tool - just do it."
)


def ask_claude(messages):
    """One call to the API. Same function is used for both turns below."""
    return client.messages.create(
        model=MODEL,
        max_tokens=300,
        system=SYSTEM,
        tools=TOOLS,
        messages=messages,
    )



def main():
    if not os.environ.get("ANTHROPIC_API_KEY"):
        print("No ANTHROPIC_API_KEY in this terminal. See README.md, step 3.")
        return

    print(f"Porter gateway running on {MODEL}.")
    print("Type a command, or 'quit' to stop.\n")

    while True:
        said = input("you    > ").strip()
        if said.lower() in ("quit", "exit", "q", ""):
            print("Stopped.")
            return

        messages = [{"role": "user", "content": said}]
        reply = ask_claude(messages)

        while reply.stop_reason == "tool_use":
            tool_call = next(b for b in reply.content if b.type == "tool_use")
            print(f"    [claude asks] {tool_call.name}({json.dumps(tool_call.input)})")

            result = set_light(on=bool(tool_call.input["on"]))

            messages.append({"role": "assistant", "content": reply.content})
            messages.append(
                {
                    "role": "user",
                    "content": [
                        {
                            "type": "tool_result",
                            "tool_use_id": tool_call.id,
                            "content": result,
                        }
                    ],
                }
            )
            reply = ask_claude(messages)

        for block in reply.content:
            if block.type == "text":
                print(f"porter > {block.text}\n")


if __name__ == "__main__":
    main()
