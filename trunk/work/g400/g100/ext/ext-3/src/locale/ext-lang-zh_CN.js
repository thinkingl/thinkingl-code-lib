/*
 * Simplified Chinese translation
 * By DavidHu
 * 09 April 2007
 * 
 * update by andy_ghg
 * 2009-10-22 15:00:57
 */

Ext.UpdateManager.defaults.indicatorText = '<div class="loading-indicator">������...</div>';

if(Ext.DataView){
   Ext.DataView.prototype.emptyText = "";
}

if(Ext.grid.GridPanel){
   Ext.grid.GridPanel.prototype.ddText = "ѡ���� {0} ��";
}

if(Ext.TabPanelItem){
   Ext.TabPanelItem.prototype.closeText = "�رմ˱�ǩ";
}

if(Ext.form.Field){
   Ext.form.Field.prototype.invalidText = "����ֵ�Ƿ�";
}

if (Ext.LoadMask) {
    Ext.LoadMask.prototype.msg = "��ȡ��...";
}

Date.monthNames = [
   "һ��",
   "����",
   "����",
   "����",
   "����",
   "����",
   "����",
   "����",
   "����",
   "ʮ��",
   "ʮһ��",
   "ʮ����"
];

Date.dayNames = [
   "��",
   "һ",
   "��",
   "��",
   "��",
   "��",
   "��"
];

Date.formatCodes.a = "(this.getHours() < 12 ? '����' : '����')";
Date.formatCodes.A = "(this.getHours() < 12 ? '����' : '����')";

if(Ext.MessageBox){
   Ext.MessageBox.buttonText = {
      ok     : "ȷ��",
      cancel : "ȡ��",
      yes    : "��",
      no     : "��"
   };
}

if(Ext.util.Format){
   Ext.util.Format.date = function(v, format){
      if(!v) return "";
      if(!(v instanceof Date)) v = new Date(Date.parse(v));
      return v.dateFormat(format || "y��m��d��");
   };
}

if(Ext.DatePicker){
   Ext.apply(Ext.DatePicker.prototype, {
      todayText         : "����",
      minText           : "���ڱ��������С��������",//update
      maxText           : "���ڱ���С�������������",//update
      disabledDaysText  : "",
      disabledDatesText : "",
      monthNames        : Date.monthNames,
      dayNames          : Date.dayNames,
      nextText          : '�¸��� (Ctrl+Right)',
      prevText          : '�ϸ��� (Ctrl+Left)',
      monthYearText     : 'ѡ��һ���� (Control+Up/Down ���ı����)',//update
      todayTip          : "{0} (�ո��ѡ��)",
      format            : "y��m��d��",
      okText            : "ȷ��",
      cancelText        : "ȡ��"
   });
}

if(Ext.PagingToolbar){
   Ext.apply(Ext.PagingToolbar.prototype, {
      beforePageText : "��",//update
      afterPageText  : "ҳ,�� {0} ҳ",//update
      firstText      : "��һҳ",
      prevText       : "��һҳ",//update
      nextText       : "��һҳ",
      lastText       : "���ҳ",
      refreshText    : "ˢ��",
      displayMsg     : "��ʾ {0} - {1}������ {2} ��",//update
      emptyMsg       : 'û������'
   });
}

if(Ext.form.TextField){
   Ext.apply(Ext.form.TextField.prototype, {
      minLengthText : "�����������С������ {0} ���ַ�",
      maxLengthText : "�����������󳤶��� {0} ���ַ�",
      blankText     : "��������Ϊ������",
      regexText     : "",
      emptyText     : null
   });
}

if(Ext.form.NumberField){
   Ext.apply(Ext.form.NumberField.prototype, {
      minText : "�����������Сֵ�� {0}",
      maxText : "������������ֵ�� {0}",
      nanText : "{0} ������Ч��ֵ"
   });
}

if(Ext.form.DateField){
   Ext.apply(Ext.form.DateField.prototype, {
      disabledDaysText  : "����",
      disabledDatesText : "����",
      minText           : "������������ڱ����� {0} ֮��",
      maxText           : "������������ڱ����� {0} ֮ǰ",
      invalidText       : "{0} ����Ч������ - ������ϸ�ʽ�� {1}",
      format            : "y��m��d��"
   });
}

if(Ext.form.ComboBox){
   Ext.apply(Ext.form.ComboBox.prototype, {
      loadingText       : "������...",
      valueNotFoundText : undefined
   });
}

if(Ext.form.VTypes){
   Ext.apply(Ext.form.VTypes, {
      emailText    : '������������ǵ����ʼ���ַ����ʽ�磺 "user@example.com"',
      urlText      : '�������������URL��ַ����ʽ�磺 "http:/'+'/www.example.com"',
      alphaText    : '��������ֻ�ܰ��������ĸ��_',//update
      alphanumText : '��������ֻ�ܰ��������ĸ,���ֺ�_'//update
   });
}
//add HTMLEditor's tips by andy_ghg
if(Ext.form.HtmlEditor){
  Ext.apply(Ext.form.HtmlEditor.prototype, {
    createLinkText : '��ӳ�������:',
    buttonTips : {
      bold : {
        title: '���� (Ctrl+B)',
        text: '��ѡ�е���������Ϊ����',
        cls: 'x-html-editor-tip'
      },
      italic : {
        title: 'б�� (Ctrl+I)',
        text: '��ѡ�е���������Ϊб��',
        cls: 'x-html-editor-tip'
      },
      underline : {
        title: '�»��� (Ctrl+U)',
        text: '����ѡ���ּ��»���',
        cls: 'x-html-editor-tip'
      },
      increasefontsize : {
        title: '��������',
        text: '�����ֺ�',
        cls: 'x-html-editor-tip'
      },
      decreasefontsize : {
        title: '��С����',
        text: '��С�ֺ�',
        cls: 'x-html-editor-tip'
      },
      backcolor : {
        title: '�Բ�ͬ��ɫͻ����ʾ�ı�',
        text: 'ʹ���ֿ���ȥ������ӫ������˱��һ��',
        cls: 'x-html-editor-tip'
      },
      forecolor : {
        title: '������ɫ',
        text: '����������ɫ',
        cls: 'x-html-editor-tip'
      },
      justifyleft : {
        title: '�����',
        text: '�����������',
        cls: 'x-html-editor-tip'
      },
      justifycenter : {
        title: '����',
        text: '�����־��ж���',
        cls: 'x-html-editor-tip'
      },
      justifyright : {
        title: '�Ҷ���',
        text: '�������Ҷ���',
        cls: 'x-html-editor-tip'
      },
      insertunorderedlist : {
        title: '��Ŀ����',
        text: '��ʼ������Ŀ�����б�',
        cls: 'x-html-editor-tip'
      },
      insertorderedlist : {
        title: '���',
        text: '��ʼ��������б�',
        cls: 'x-html-editor-tip'
      },
      createlink : {
        title: 'ת�ɳ�������',
        text: '����ѡ�ı�ת���ɳ�������',
        cls: 'x-html-editor-tip'
      },
      sourceedit : {
        title: '������ͼ',
        text: '�Դ������ʽչ���ı�',
        cls: 'x-html-editor-tip'
      }
    }
  });
}


if(Ext.grid.GridView){
   Ext.apply(Ext.grid.GridView.prototype, {
      sortAscText  : "����",//update
      sortDescText : "����",//update
      lockText     : "������",//update
      unlockText   : "�������",//update
      columnsText  : "��"
   });
}

if(Ext.grid.PropertyColumnModel){
   Ext.apply(Ext.grid.PropertyColumnModel.prototype, {
      nameText   : "����",
      valueText  : "ֵ",
      dateFormat : "y��m��d��"
   });
}

if(Ext.layout.BorderLayout && Ext.layout.BorderLayout.SplitRegion){
   Ext.apply(Ext.layout.BorderLayout.SplitRegion.prototype, {
      splitTip            : "�϶����ı�ߴ�.",
      collapsibleSplitTip : "�϶����ı�ߴ�. ˫������."
   });
}