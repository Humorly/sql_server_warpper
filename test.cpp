
#include <iostream>
#include "sql_server_warpper.h"
#include <tchar.h>

int main()
{
	sql_server_warpper sql_(std::wstring(TEXT("PROVIDER=SQLOLEDB;DATABASE=master;Data Source=127.0.0.1,1456;")),
		std::wstring(TEXT("admin")), std::wstring(TEXT("aaa123")));

	std::tuple<std::wstring, std::wstring> params_(TEXT("userid"), TEXT("name"));
	std::vector<std::tuple<int, int>> content_;
	sql_.select(std::wstring(TEXT("select * from game.dbo.gameuserinfo;")), content_, params_);
	
	for (auto i = 0; i < content_.size(); ++i)
	{
		auto val = content_[i];
		std::cout << std::get<0>(val) << " + " << std::get<1>(val) << "\n";
	}
	
	return 0;
}








