#include <iostream>
#include "sql_server_warpper.h"

int main()
{
	// 配置用户名、密码、地址、端口
	sql_server_warpper sql_(std::wstring(TEXT("PROVIDER=SQLOLEDB;DATABASE=master;Data Source=127.0.0.1,9977;")),
		std::wstring(TEXT("root")), std::wstring(TEXT("778779")));

	// 创建
	sql_.invoke(std::wstring(TEXT("create table game.dbo.test (name VARCHAR(50) NOT NULL, id INT NOT NULL);")));
	// 增加
	sql_.insert(std::wstring(TEXT("insert into game.dbo.test (name, id) values('hello', 123);")));
	// 删除
	sql_.remove(std::wstring(TEXT("delete top(1) from game.dbo.test;")));
	// 修改
	sql_.update(std::wstring(TEXT("update game.dbo.test set id = 2000000 where name = 'hello';")));

	// 查询
	std::tuple<std::wstring, std::wstring> params_(TEXT("name"), TEXT("id"));
	std::vector<std::tuple<std::wstring, int>> content_;
	sql_.select(std::wstring(TEXT("select * from game.dbo.test;")), content_, params_);
	for (std::size_t i = 0; i < content_.size(); ++i)
	{
		auto val = content_[i];
		std::wcout << std::get<0>(val) << " + " << std::get<1>(val) << "\n";
	}

	return 0;
}

