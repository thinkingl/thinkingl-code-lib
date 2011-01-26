
<p class="title">DVR列表</p>
          <table width="100%" border="0" cellpadding="0" cellspacing="0">
<tr align="center"><td width="10%" height="38" align="left" valign="middle" style="color:#205699; font-family:'宋体'; font-size:12px; padding-left:9px; padding-top:3px">DVR状态</td><td width="90%" align="left" 
valign="middle" style="padding-top:3px"><input type="text" id="dvrlist_status"/></td></tr></table>

        <p class="select"><div id="dvrlist"></div></p>
        <input type="button" class="leftFloat  mainButton mainButton_left" id="btn_stopDvr"  value="断开" />
        <input type="button" class="leftFloat  mainButton" id="btn_startDvr"  value="连接" />
        <input type="button" class="leftFloat  mainButton" id="btn_detailDvr"  value="详细信息" />
        <input type="button" class="rightFloat  mainButton" id="btn_deleteDvr"  value="删除" />
        <input type="button" class="rightFloat  mainButton" id="btn_editDvr"  value="修改" />
        <input type="button" class="rightFloat  mainButton" id="btn_addDvr"  value="添加" />
		<input type="button" class="rightFloat  mainButton" id="btn_advanceDvr"  value="高级配置" />
        <p>&nbsp;</p>
		<p class="title">统计系统</p>
        <p>DVR接入的总数量：<span id="dvrlist_total">0</span></p>
        <p>DVR正常连接数量：<span id="dvrlist_normal">0</span></p>
        <p>DVR连接失败数量：<span id="dvrlist_faild2dvr">0</span></p>
        <p>DVR未启动的数量：<span id="dvrlist_noStart">0</span></p>
        <p>连接平台失败数量：<span id="dvrlist_faild2cmu">0</span></p>
        <div id="alladddvr"></div>
