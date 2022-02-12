#include "route.h"
#include <glog/logging.h>

void Route::deliver( std::shared_ptr<XMessage> msg )
{
    LOG(INFO) << "route will deliver msg: " << msg->toJson();
}