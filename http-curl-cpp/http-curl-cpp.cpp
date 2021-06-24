#define BUILDING_LIBCURL // 解决：无法解析的外部符号 __imp__curl_easy_init

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream> // 提供：cout、string
#include <curl/curl.h> // 提供：curl
#include <json/json.h> // 提供：json

using namespace std;

// 数据的获取
// 参见：https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
static size_t cb(void* data, size_t size, size_t nmemb, void* userp)
{
	string* str = dynamic_cast<string*>((string*)userp);

	if (0 == size || NULL == data) {
		return -1;
	}

	char* char_data = (char*)data;
	str->append(char_data, nmemb * size);
	return nmemb * size;
}

// char 复制
void char_copy(char* target, const char* source) {
	int i = 0;
	while ((target[i] = source[i]) != '\0') { // (a[i] = b[i]) = b[i]
		i++;
	}
}

#ifdef _WIN32
static int multi2uni(const string& multi, wstring& uni, UINT code)
{
	auto len = MultiByteToWideChar(code, 0, multi.c_str(), -1, nullptr, 0);
	if (len <= 0)
	{
		cerr << __FILE__ << " : " << __LINE__ << " : " << GetLastError() << endl;
		return -1;
	}
	WCHAR* buf = new WCHAR[len];
	if (buf == nullptr)
	{
		cerr << __FILE__ << " : " << __LINE__ << " : " << "can not new buf, size : " << len << endl;
		return -2;
	}
	len = MultiByteToWideChar(code, 0, multi.c_str(), -1, buf, len);
	uni.assign(buf);
	delete[]buf;
	buf = nullptr;
	return len;
}

static int uni2multi(const wstring& uni, string& multi, UINT code)
{
	auto len = WideCharToMultiByte(code, 0, uni.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (len <= 0)
	{
		cerr << __FILE__ << " : " << __LINE__ << " : " << GetLastError() << endl;
		return -1;
	}
	CHAR* buf = new CHAR[len];
	if (buf == nullptr)
	{
		cerr << __FILE__ << " : " << __LINE__ << " : " << "can not new buf, size : " << len << endl;
		return -2;
	}
	len = WideCharToMultiByte(code, 0, uni.c_str(), -1, buf, len, nullptr, nullptr);
	multi.assign(buf);
	delete[]buf;
	buf = nullptr;
	return len;
}
#endif

// ANSI->Unicode
int ansi2uni(const string& ansi, wstring& uni)
{
#ifdef _WIN32
	return multi2uni(ansi, uni, CP_ACP);
#endif
	return 0;
}

// Unicode->ANSI
int uni2ansi(const wstring& uni, string& ansi)
{
#ifdef _WIN32
	return uni2multi(uni, ansi, CP_ACP);
#endif
	return 0;
}

// UTF8->Unicode
int utf82uni(const string& utf8, wstring& uni)
{
#ifdef _WIN32
	return multi2uni(utf8, uni, CP_UTF8);
#endif
	return 0;
}

// Unicode->UTF8
int uni2utf8(const wstring& uni, string& utf8)
{
#ifdef _WIN32
	return uni2multi(uni, utf8, CP_UTF8);
#endif
	return 0;
}

// ANSI->UTF8
int ansi2utf8(const string& ansi, string& utf8)
{
	wstring uni;
	auto len = ansi2uni(ansi, uni);
	if (len <= 0)
	{
		return -3;
	}
	return uni2utf8(uni, utf8);
}

// UTF8->ANSI
int utf82ansi(const string& utf8, string& ansi)
{
	wstring uni;
	auto len = utf82uni(utf8, uni);
	if (len <= 0)
	{
		return -3;
	}
	return uni2ansi(uni, ansi);
}

// http 请求
int http(char url[1024], int method, char body[1024], char response[1024])
{
	int code; // curl 代码
	int http_code;
	int header_size;
	string response_string;

	char* version = curl_version(); // libcurl 版本
	cout << "curl_version：" << version << endl;

	CURL* curl_handle = curl_easy_init(); // 启动 libcurl 简单会话，参见：https://curl.se/libcurl/c/curl_easy_init.

	code = curl_easy_setopt(curl_handle, CURLOPT_URL, url); // 设置 URL，参见：https://curl.se/libcurl/c/curl_easy_setopt.html
	code = curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, cb); // 将所有数据发送到此函数，参见：https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
	code = curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&response_string); // 我们将‘chunk’结构传递给回调函数，参见：https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html	

	if (method == 1)
	{
		// GET 请求

	}
	else if (method == 2) {
		// POST 请求

		// Header
		struct curl_slist* headers = NULL;
		headers = curl_slist_append(headers, "content-type:application/json");
		code = curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

		code = curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, body); // 设置POST的数据
	}
	else {
		// 使用 GET 请求

	}

	code = curl_easy_perform(curl_handle); // 执行阻塞文件传输，参见：https://curl.se/libcurl/c/curl_easy_perform.html

	code = curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &http_code); // 获取 http 响应代码，参见：https://curl.se/libcurl/c/CURLINFO_RESPONSE_CODE.html ，参见：https://everything.curl.dev/libcurl-http/responses#http-response-code
	code = curl_easy_getinfo(curl_handle, CURLINFO_HEADER_SIZE, &header_size);

	curl_easy_cleanup(curl_handle); // 结束一个 libcurl 简单句柄，参见：https://curl.se/libcurl/c/curl_easy_cleanup.html

	cout << "header_size：" << header_size << endl;

	if (code == CURLcode::CURLE_OK)
	{
		// curl 正常

		string response_ansi_string;

		// 编码转换
		utf82ansi(response_string, response_ansi_string);

		const char* response_ansi_char = response_ansi_string.data();

		char_copy(response, response_ansi_char);

		if (http_code == 200)
		{
			// http 正常

			Json::CharReaderBuilder readerBuilder;
			Json::CharReader* reader(readerBuilder.newCharReader());
			Json::Value root;
			JSONCPP_STRING errs;
			bool ok = reader->parse(response_ansi_char, response_ansi_char + strlen(response_ansi_char), &root, &errs);
			if (ok && errs.size() == 0)
			{
				string msg = root["msg"].asString();

				cout << "msg：" << msg << endl;
			}
			delete reader;
		}

	}

	if (code == 0)
	{
		return http_code;
	}
	else {
		return code;
	}
}


int main()
{

	char url[] = "https://cicd.jiajiakang.net/internet-hospital/common/currentTimeMillis";
	int method = 2;
	char body[] = "{\"msg\"=\"hello\"}";

	char response[1024]; // http 响应

	int curl_code = http(url, method, body, response);

	cout << endl;
	cout << "curl 调用结束：" << endl;
	cout << "curl_code：" << curl_code << endl;
	cout << "response：" << response << endl;

	cout << endl;
	cout << "curl 响应代码：" << endl;
	cout << CURLE_OK << "：正常" << endl;
	cout << CURLE_COULDNT_CONNECT << "：无法连接" << endl;

	cout << endl;
	cout << "http 响应代码：" << endl;
	cout << 200 << "：正常" << endl;
	cout << 404 << "：未发现" << endl;
	cout << 405 << "：方法不允许" << endl;
	cout << 500 << "：异常" << endl;
	cout << "其他代码：请参考：https://baike.baidu.com/item/HTTP%E7%8A%B6%E6%80%81%E7%A0%81/5053660" << endl;

	return 0;
}
