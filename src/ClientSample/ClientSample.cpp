 // ClientSample.cpp

#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

int main(void) 
{
	const WCHAR destination[] = L"127.0.0.1";	//接続するサーバのIPアドレス
	const int PORT_NO = 7000;	//ポート番号はサーバプログラムと共通

	//ソケット通信の準備
	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);

	// サーバのアドレス情報を設定
	struct sockaddr_in dest;
	memset(&dest, 0, sizeof(dest));
	dest.sin_port = htons(PORT_NO);
	dest.sin_family = AF_INET;
	InetPton(dest.sin_family, destination, &dest.sin_addr.S_un.S_addr);

	// ソケットの生成
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

	//サーバへの接続
	if (connect(s, (sockaddr *)&dest, sizeof(dest))) {
		printf("%lsに接続できませんでした\n", destination);
		return -1;
	}
	printf("接続しました: %ls\n", destination);

	while(1){
		// サーバにデータを送信
		printf("サーバに送信する文字列を入力して下さい(「bye」なら終了)\n");

		char send_data[1024];
		scanf_s("%s", send_data, 1024);
		if (send(s, send_data, strlen(send_data) + 1, 0) < 0) {
			printf("送信エラー\n");
		}

		// "bye"で終了
		if (strcmp("bye", send_data) == 0) break;

		// サーバからデータを受信
		char buffer[1024];
		if (recv(s, buffer, sizeof(buffer), 0) < 0) {
			printf("受信エラー\n");
		}
		printf("応答がきました: %s\n", buffer);
	};

	// ソケット通信の終了
	printf("接続終了\n");
	closesocket(s);

	return 0;
}
