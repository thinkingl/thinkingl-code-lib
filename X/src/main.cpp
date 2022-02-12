#include <iostream>

#include "asio.hpp"
#include "gtest/gtest.h"
#include "glog/logging.h"
#include "gflags/gflags.h"
#include <string>
#include "xnode.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include "route.h"

using namespace std;
using namespace google;
using namespace nlohmann;

json initConfig()
{
	const char* cfgPath = "xconfig.json";
	ifstream ifCfg( cfgPath );
	if( ifCfg )
	{
		json j;
		ifCfg >> j;
		return j;
	}
	else
	{
		ofstream ofCfg( cfgPath );
		json j = { 
			{"xnode",
				{
					{"id",""},
					{"name",""},
					{"services",
						{}
						}
				}
			}
			};
		j = R"(
			{
				"xnode":
					{
						"id":"",
						"name":"",
						"services":
							[

							]
					}
			}
		)"_json;
		ofCfg << std::setw(4) << j;
		return j;
	}
}

int main(int argc, char *argv[])
{
	// todo gflags的链接错误。
	//google::ParseCommandLineFlags(&argc, &argv,true);
	FLAGS_log_dir = "./logs";
	FLAGS_colorlogtostderr=true;  //设置输出颜色
	FLAGS_alsologtostderr =true;//设置日志消息除了日志文件之外是否去标准输出
	//FLAGS_v = 5;
	google::InitGoogleLogging( argv[0] );

	LOG(INFO) << "X run!";

    asio::io_context io_context;

	if( argc > 1 )
	{
		string type = argv[1];
		if( type == "test" )
		{
			LOG(INFO) << "test mode";
			::testing::InitGoogleTest(&argc, argv );

			google::ShutdownGoogleLogging();
			return RUN_ALL_TESTS();
		}
	}

	json config = initConfig();
	LOG(INFO) << "config: " << config.dump(4);

	Route route;
	auto node = std::make_shared<XNode>(io_context, config["xnode"], route);
	node->run();

	asio::error_code er;
	io_context.run( er );

	LOG(INFO) << "X exit";

	google::ShutdownGoogleLogging();
	return 0;
}