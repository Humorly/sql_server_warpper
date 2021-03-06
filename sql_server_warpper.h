#pragma once

#include "sql_server_warpper_impl.h"

class sql_server_warpper : public sql_server_warpper_ipml
{
public:
	sql_server_warpper(std::wstring path, std::wstring user, std::wstring password) 
	{
		content_.open(path, user, password);
	}
	virtual ~sql_server_warpper() {}

	// 增加
	bool insert(const std::wstring& command)
	{
		return invoke(command);
	}

	// 删除
	bool remove(const std::wstring& command)
	{
		return invoke(command);
	}

	// 修改
	bool update(const std::wstring& command)
	{
		return invoke(command);
	}

	// 查询
	template <typename __set, typename __type, typename ... params>
	bool select(const std::wstring& command, std::vector<std::tuple<__type, params...>>& dest, __set parm)
	{
		_RecordsetPtr record_;

		try
		{
			record_ = content_.execute(command);
		}
		catch (_com_error & e)
		{
			std::wcout << "sql error is --> " << e.Description() << "\n";
			return false;
		}

		// 没有内容
		if (record_->adoEOF) return false;

		// 读取内容
		while (!record_->adoEOF)
			separation<std::tuple<__type, params...>, _RecordsetPtr, __set, __type, params...>(dest, record_, parm);

		record_->Close();
		record_ = nullptr;

		return true;
	}

	// 执行过程
	bool invoke(const std::wstring& command)
	{
		try
		{
			auto& record_ = content_.execute(command);
		}
		catch (_com_error & e)
		{
			std::wcout << "sql error is --> " << e.Description() << "\n";
			return false;
		}

		return true;
	}

private:
	struct sql_content
	{
	public:
		sql_content()
		{
			// 初始实例
			init_com_instance();
		}

		virtual ~sql_content() { close(); }

		// 连接数据库
		int open(std::wstring path, std::wstring user, std::wstring password)
		{
			try
			{
				auto ret_ = cn_->Open(path.c_str(), user.c_str(), password.c_str(), adConnectUnspecified);
				return ret_;
			}
			catch (_com_error & e)
			{
				std::wcout << "sql error is --> " << e.Description() << "\n";
				return 0;
			}
			return 0;
		}

		// 执行语句
		_RecordsetPtr & execute(std::wstring command)
		{
			rs_ = cn_->Execute(command.c_str(), NULL, adCmdText);
			return rs_;
		}

	private:
		bool init_ = false;
		_ConnectionPtr cn_ = nullptr;
		_RecordsetPtr rs_ = nullptr;

		// 初始实例
		void init_com_instance()
		{
			// 启动标识
			if (!init_)
			{
				CoInitialize(NULL);
				cn_ = _ConnectionPtr("ADODB.Connection");
				init_ = true;
			}
		}

		void close()
		{
			if (init_)
			{
				// 移除连接
				if (nullptr != cn_) cn_->Close();

				cn_ = nullptr;
				rs_ = nullptr;
				CoUninitialize();
			}
		}
	};

	// sql连接对象
	sql_content content_;
};
