#ifndef UTILS_HPP
# define UTILS_HPP

#include<iostream>
#include<sstream>

std::vector<std::string> split(std::string input, char delimiter);

#endif


/*
class Parser
클라이언트로 부터 전달받은 문자열을 받아 어떤 커맨드인지, 커맨드의 인자는 어떤 것이 있는지, 커맨드의 인자가 유효한지 검사하는 클래스
검사한 다음 서버로 해당 정보를 넘겨주어야 하는데 이 부분은 어떻게?
서버로 넘겨 서버쪽에서 처리를 하거나, 파서에서 서버의 메소드를 불러와 처리를 하거나 어쨌든 다른 객체들에 접근을 해야 함.
프로그램 내내 자주 쓰이는 객체들의 주소값을 저장하고있는 전역변수 같은 게 하나 필요할 듯?

각종 시그널 처리도 필요할 듯
이 부분은 평가 재량일 것 같기는 한데 어느정도 다뤄야 할 것 같음.
*/