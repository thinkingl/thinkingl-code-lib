function goToDvrDetails(){
	$(".rightTab").html("<p class='title'>DVR详细信息</p>"+
    "<label>名称："+
  "<input type='text' name='textfield' />"+
  "</label>"+
  "<p>"+
    "<label>PUID:"+
    "<input type='text' name='textfield2' />"+
    "</label>"+
  "</p>"+
  "<p>"+
    "<label>DVR IP:"+
    "<input type='text' name='textfield3' />"+
    "</label>"+
    "<label>端口"+
    "<input type='text' name='textfield4' />"+
    "</label>"+
  "</p>"+
  "<p>"+
    "<label>通道数"+
    "<input type='text' name='textfield5' />"+
    "</label>"+
  "</p>"+
  "<p>"+
    "<label>用户名："+
    "<input type='text' name='textfield6' />"+
    "</label>"+
    "<label>密码："+
    "<input type='text' name='textfield7' />"+
    "</label>"+
  "</p>"+
  "<p>"+
    "<label>厂商："+
    "<select name='select'>"+
    "</select>"+
    "</label>"+
  "</p>"+
  "<p>配置文件名：mtg.ini</p>"+
  "<p>进程信息：dvrproxy(45621)</p>"+
  "<p>当前状态：正常</p>"+
  "<p>telnet端口：2501</p>"+
  "<input type='submit' name='Submit' value='保存 />");
 }
