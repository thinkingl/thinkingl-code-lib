<p class='title'>网络参数</p>

<table width='100%' height='120' border='0' class='rightTabCharater'>
  <tr height='30'>
    <td width='23%'>
      <div align='right'>网关别名 ：</div>
    </td>
    <td width='77%'>
      <div id='g400_name'></div>
    </td>
  </tr>

  <tr height='30'>
    <td width='23%'>
      <div align='right'>监控网段网卡 ：</div>
    </td>
    <td width='77%'>
      <div id='g400_kdm_eth'></div>
    </td>
  </tr>

  <tr height='30'>
    <td width='23%'>
      <div align='right'>会议网段网卡 ：</div>
    </td>
    <td width='77%'>
      <div id='g400_kdv_eth'></div>
    </td>
  </tr>
  
  <tr height='30'>
    <td width='23%'>
      <div align='right'>网卡1 ：</div>
    </td>
    <td width='77%'>
      <div id='eth0_name'></div>
    </td>
  </tr>
  <tr height='20' >
    <td width='23%'>
      <div align='right'>IP地址 ：</div>
    </td>
    <td width='77%'>
      <div id='eth0_ip'></div>
    </td>
  </tr>
  <tr height='20'>
    <td>
      <div align='right'>子网掩码：</div>
    </td>
    <td>
      <div id='eth0_mask'></div>
    </td>
  </tr>
  <tr height='20'>
    <td>
      <div align='right'>网关：</div>
    </td>
    <td>
      <div id='eth0_gateway'></div>
    </td>
  </tr>

  <tr height='30'>
    <td width='23%'>
      <div align='right'>网卡2 ：</div>
    </td>
    <td width='77%'>
      <div id='eth1_name'></div>
    </td>
  </tr>
  <tr height='20' >
    <td width='23%'>
      <div align='right'>IP地址 ：</div>
    </td>
    <td width='77%'>
      <div id='eth1_ip'></div>
    </td>
  </tr>
  <tr height='20'>
    <td>
      <div align='right'>子网掩码：</div>
    </td>
    <td>
      <div id='eth1_mask'></div>
    </td>
  </tr>
  <tr height='20'>
    <td>
      <div align='right'>网关：</div>
    </td>
    <td>
      <div id='eth1_gateway'></div>
    </td>
  </tr>

  <tr height='30'>
    <td width='23%'>
      <div align='right'>网卡3 ：</div>
    </td>
    <td width='77%'>
      <div id='eth2_name'></div>
    </td>
  </tr>
  <tr height='20' >
    <td width='23%'>
      <div align='right'>IP地址 ：</div>
    </td>
    <td width='77%'>
      <div id='eth2_ip'></div>
    </td>
  </tr>
  <tr height='20'>
    <td>
      <div align='right'>子网掩码：</div>
    </td>
    <td>
      <div id='eth2_mask'></div>
    </td>
  </tr>
  <tr height='20'>
    <td>
      <div align='right'>网关：</div>
    </td>
    <td>
      <div id='eth2_gateway'></div>
    </td>
  </tr>
 
  
  
  <tr >
    <td>
      <div align='right'> </div>
    </td>
    <td>
      <input type='button' name='btn_change' class='rightFloat mainButton mainButton_normal' id='btn_net_change' value='修改'/>
      <input type='button' name='btn_apply' class='rightFloat mainButton mainButton_normal' id='btn_net_apply' value='应用' />
      <input type='button' name='btn_apply' class='rightFloat mainButton mainButton_normal' id='btn_net_cancel' value='取消' />
    </td>
  </tr>
</table>

<p class='title'>会议参数</p>
<table width='100%' height='239' border='0' class='rightTabCharater'>
      <tr>
        <td width='23%' height='25'><div align='right'>GK注册状态：</div></td>
        <td width='77%' height='25'><div id='gk_status'></div></td>
      </tr>
      <tr>
        <td height='25'><div align='right'>并发入会能力（1-64） ：</div></td>
        <td height='25'><div id='gk_max_meet'></div></td>
      </tr>  
      <tr>
        <td height='30'>&nbsp;</td>
        <td height='30'>
          <input name='checkbox1' type='checkbox' class='mainEdit Nat' id='gk_reg' />
          <label class = 'mainEdit Nat' id='label_gk_reg' >注册到GK</label>
          <div id='gk_reg_status'></div>
        </td>
      </tr>
      <tr>
        <td height='25'><div align='right'>GK地址：</div></td>
        <td height='25'>
        <div id='gk_ip'></div></td>
      </tr>
      <tr>
        <td height='25'><div align='right'>呼叫端口：</div></td>
        <td height='25'><div id='gk_call_port'></div></td>
      </tr>
      <tr>
        <td height='25'>
          <div align='right'>RAS端口：</div>
        </td>
        <td height='25'>
          <div id='gk_ras_port'></div>
        </td>
      </tr>
      
      <tr>
        <td height='25'><div align='right'></div></td>
        <td height='25'>
		<div class='leftFloat_down'>
       
		</div>
          <input name='btn_change' type='button' class='rightFloat mainButton mainButton_normal' id='btn_gk_change' value='修改'/>
          <input name='btn_apply' type='button' class='rightFloat mainButton mainButton_normal' id='btn_gk_apply' value='应用' />
          <input name='btn_apply' type='button' class='rightFloat mainButton mainButton_normal' id='btn_gk_cancel' value='取消' />
        </td>
      </tr>
      <tr>
        <td height='21'>&nbsp;</td>
        <td height='21'>&nbsp;</td>
      </tr>
</table>
	
<p>&nbsp;</p>
<p>&nbsp;</p>
<p>&nbsp;&nbsp;</p>
<p>&nbsp;</p>
<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</p>
<p align='right'>&nbsp;</p>