#pragma once

#include "portabledefine.h"

/** xml message define */

#define DEF_MSG static ctstring
#define DEF_ELE static ctstring

/*
 * <KedacomXMLData>
  <Head>
    <Msg>MSG_LOGIN</Msg>
    <Sn>1</Sn>
    <Session>2011021615555200</Session>
    <Status> 1 </Status>
  </Head>
  <Content>
    <ErrorMesssage >登录成功</ErrorMesssage>
  </Content>
</KedacomXMLData>
 */

/** xml message element names */
namespace G400XML
{
	DEF_ELE ELE_ROOT = "KedacomXMLData";

	DEF_ELE ELE_HEAD = "Head";
	DEF_ELE ELE_SN = "Sn";
	DEF_ELE ELE_SESSION = "Session";
	DEF_ELE ELE_STATUS = "Status";

	DEF_ELE ELE_CONTENT = "Content";
	DEF_ELE ELE_ERRMSG = "ErrorMesssage";
};



/** G400 XML Message IDs. */
namespace G400Message
{
	DEF_MSG MSG_LOGIN = "MSG_LOGIN";

};

/** */
namespace G400Status
{
	enum EG400Status
	{
		UNKNOWN_ERR,
		OK,
	};
};





