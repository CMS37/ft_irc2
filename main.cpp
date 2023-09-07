#include "Server.hpp"

// void a(void)
// {
// 	system("leaks server");
// }


int main(int argc, char **argv)
{
	Server server;

	// atexit(a);
	try
	{
		server.input(argc, argv);
		server.run();
	}
	catch(const std::exception& e)
	{
		close(server.getFd());
		std::cerr << e.what() << std::endl;
		return (1);
	}

	return (0);
}


// server이름 길이는 최대 63자로
// 사용자이름 최대 길이는 9자
// 채널이름은 최대 50자 대소문자 구분 x, 첫번쨰 문자는(&', '#', '+','!') 공백있으면안댐
// & : Channels with '&' as prefix are local to the server where they are created.(???관리자채널인가)
// + : 채널모드 지원x (운영자도 없음)
// # : 일반채널
// ! : 채널생성 및 생성자가 채널운영자가됨
//         INVITE - 초대 전용 채널(모드 +i)에 클라이언트 초대 
//         KICK - 채널에서 클라이언트 퇴출 
//         TOPIC - 모드에서 채널 주제 변경 +t 채널 
//         MODE - 채널 모드 및 구성원의 권한 변경
//		         +o : 채널 운영자 권한 부여/회수
//               +i : 초대전용 채널 설정/해제
//               +k : 채널 비밀번호 설정/해제
//               +t : 채널 주제 변경 권한 설정/해제
//               +l : 채널 최대 사용자 수 설정/해제

// {}|^ 문자들은 []\~ 문자와 동등한 소문자로 취급한다.

// 전달되는 메세지는 512자 이하로 제한한다. = 명령 및 해당매개변수를 제외하면 510자

// client input example
// /server <servername> [port] : 서버에 연결
// /nick <nickname> : 닉네임 변경
// /list : 채널 목록 출력
// /join <channelname> : 채널에 참여
// /part <channelname> : 채널에서 나가기
// /users : 채널에 참여한 사용자 목록 출력
// /names <channelname> : 채널에 참여한 사용자 목록 출력
// /msg <nickname> <message> : 특정 사용자에게 귓속말
// /quit : 서버 연결 종료
// /help : 도움말 출력
// /time : 서버의 시간 출력
// /version : 서버의 버전 출력
// /motd : 서버의 메시지 출력
// /invite <nickname> <channelname> : 특정 사용자를 채널에 초대
// /kick <nickname> <channelname> : 특정 사용자를 채널에서 퇴장
// /topic <channelname> <topic> : 채널의 주제 변경
// /mode <channelname> <mode> : 채널의 모드 변경
