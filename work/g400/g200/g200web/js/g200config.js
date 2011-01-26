/*EV_START对应osp的起始序号*/
var G200_ET =
{
	EV_START                                       : 4024,
	BS_G200MS_USER_LOGIN_REQ                       : eventIndex(1),
	G200MS_BS_USER_LOGIN_ACK                       : eventIndex(2),
	G200MS_BS_USER_LOGIN_NACK                      : eventIndex(3),

    BS_G200MS_ADD_MATRIX_REQ: eventIndex(4),
    G200MS_BS_ADD_MATRIX_ACK: eventIndex(5),
    G200MS_BS_ADD_MATRIX_NACK: eventIndex(6),

    BS_G200MS_DELETE_MATRIX_REQ: eventIndex(7),
    G200MS_BS_DELETE_MATRIX_ACK: eventIndex(8),
    G200MS_BS_DELETE_MATRIX_NACK: eventIndex(9),

    BS_G200MS_UPDATE_MATRIX_REQ: eventIndex(10),
    G200MS_BS_UPDATE_MATRIX_ACK: eventIndex(11),
    G200MS_BS_UPDATE_MATRIX_NACK: eventIndex(12),

    BS_G200MS_ADD_ENCODER_REQ: eventIndex(13),
    G200_BS_ADD_ENCODER_ACK: eventIndex(14),
    G200_BS_ADD_ENCODER_NACK: eventIndex(15),

    BS_G200MS_DELETE_ENCODER_REQ: eventIndex(16),
    G200MS_BS_DELETE_ENCODER_ACK: eventIndex(17),
    G200MS_BA_DELETE_ENCODER_NACK: eventIndex(18),
    
    BS_G200MS_UPDATE_ENCODER_REQ: eventIndex(19),
    G200MS_BS_UPDATE_ENCODER_ACK: eventIndex(20),
    G200MS_BS_UPDATE_ENCODER_NACK: eventIndex(21),

    BS_G200MS_ADD_CAMERA_REQ: eventIndex(22),
    G200_BS_ADD_CAMERA_ACK: eventIndex(23),
    G200_BS_ADD_CAMERA_NACK: eventIndex(24),
    
    BS_G200MS_DELETE_CAMERA_REQ: eventIndex(25),
    G200MS_BS_DELETE_CAMERA_ACK: eventIndex(26),
    G200MS_BS_DELETE_CAMERA_NACK: eventIndex(27),

    BS_G200MS_UPDATE_CAMERA_REQ: eventIndex(28),
    G200MS_BS_UPDATE_CAMERA_ACK: eventIndex(29),
    G200MS_BS_UPDATE_CAMERA_NACK: eventIndex(30),

    BS_G200MS_ADDLINK_REQ: eventIndex(31),
    G200MS_BS_ADDLINK_ACK: eventIndex(32),
    G200MS_BS_ADDLINK_NACK: eventIndex(33),

    BS_G200MS_DELLINK_REQ: eventIndex(34),
    G200MS_BS_DELLINK_ACK: eventIndex(35),
    G200MS_BS_DELLINK_NACK: eventIndex(36),

    BS_G200MS_MATRIXSINFO_REQ: eventIndex(37),
    G200MS_BS_MATRIXSINFO_ACK: eventIndex(38),
    G200MS_BS_MATRIXSINFO_NACK: eventIndex(39),

	BS_G200MS_DEVICESLIST_REQ: eventIndex(40),
    G200MS_BS_DEVICESLIST_ACK: eventIndex(41),
    G200MS_BS_DEVICESLIST_NACK: eventIndex(42),
	
    BS_G200MS_UPDATE_USER_REQ: eventIndex(43),
    G200_BS_UPDATE_USER_ACK: eventIndex(44),
    G200_BS_UPDATE_USER_NACK: eventIndex(45),
	
	BS_G200_SYSCONF_REQ: eventIndex(46),
    G200_BS_SYSCONF_ACK: eventIndex(47),
    G200_BS_SYSCONF_NACK: eventIndex(48),

    BS_G200_SYSTEMREBOOT_REQ: eventIndex(49),
    G200_BS_SYSTEMREBOOT_ACK: eventIndex(50),
    G200_BS_SYSTEMREBOOT_NACK: eventIndex(51),
	
	BS_G200_HEARTBEAT_NTF:eventIndex(52),
	G200_BS_HEARTBEAT_NTF:eventIndex(53),
	
	BS_G200_SYSPARASET_REQ:eventIndex(54),
    G200_BS_SYSPARASET_ACK:eventIndex(55),
    G200_BS_SYSPARASET_NACK:eventIndex(56),

    BS_G200_SYSIPSET_REQ:eventIndex(57),
    G200_BS_SYSIPSET_ACK:eventIndex(58),
    G200_BS_SYSIPSET_NACK:eventIndex(59),
	
	BS_G200MS_SYSMAIN_REQ:eventIndex(60),
    G200MS_BS_SYSMAIN_ACK:eventIndex(61),
    G200MS_BS_SYSMAIN_NACK:eventIndex(62),

    BS_G200MS_SYSQUIT_REQ:eventIndex(63),
    G200MS_BS_SYSQUIT_ACK:eventIndex(64),
    G200MS_BS_SYSQUIT_NACK:eventIndex(65),
	
	BS_G200MS_DEVICESTATUS_REQ:eventIndex(66),
	G200MS_BS_DEVICESTATUS_ACK:eventIndex(67),
	G200MS_BS_DEVICESTATUS_NACK:eventIndex(68),
	
	BS_G200MS_EXPORTCONFIG_REQ:eventIndex(69),
	G200MS_BS_EXPORTCONFIG_ACK:eventIndex(70),
	G200MS_BS_EXPORTCONFIG_NACK:eventIndex(71),
	
	BS_G200MS_G200_MPLATSTATUS_REQ:eventIndex(72),
	G200_G200MS_BS_MPLATSTATUS_ACK:eventIndex(73),
	G200_G200MS_BS_MPLATSTATUS_NACK:eventIndex(74),

	BS_G200MS_3ASTATUS_REQ:eventIndex(75),
	G200MS_BS_3ASTATUS_ACK:eventIndex(76),
	G200MS_BS_3ASTATUS_NACK:eventIndex(77),
	
	BS_G200MS_CAMERALIST_REQ:eventIndex(78),
	G200MS_BS_CAMERALIST_ACK:eventIndex(79),
	G200MS_BS_CAMERALIST_NACK:eventIndex(80),

	BS_G200MS_ENCODERLIST_REQ:eventIndex(81),
	G200MS_BS_ENCODERLIST_ACK:eventIndex(82),
	G200MS_BS_ENCODERLIST_NACK:eventIndex(83)
}

function eventIndex(index)
{
	return index + 4024;
}

var ERROR_CODE =
{
	1000:"错误的用户名或密码",
	1001:"用户已经登录",
	1002:"超过软件支持的最大限度",
	1003:"license无效",
	1004:"写文件失败"
};


var MPLAT_TYPE = 
{
	0:"KDM_V5R1B1SP3",
	1:"VS_V1R4B2",
	2:"VS_V1R4B3"
};