-------------------
# BUG

+ 가끔씩 라인을 읽어올때 의미없는 쓰레기값도 같이넘어오는 현상
	+ 파싱다시 만들 예정(siykim님 진행중)

+ 서버시작할때 호스트정보를 넘겨주는 작업이 필요함
```
-!- Irssi: critical nicklist_set_host: assertion 'host != NULL' failed
```

+ cmd_user
	+ REALNAME값이 안들어감
	+ localhost로 접속시 그냥 localhost로 저장이되서 이부분을 IP를 가져오는걸로
```
this->_client.setServername(this->_tokens[3]); //서버이름그냥 호스트네임으로?
```

# 남은 작업

+ 수정할거 찾기

+ WHO 커맨드 작성?
	+ 기본작성은 끝낫는데 추가 WHO할지말지는 흠..


-------------------