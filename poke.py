import requests
r = requests.get("http://192.168.1.110/light?on=1", timeout=3)
print(r.status_code)
print(r.text)
print(r.ok)