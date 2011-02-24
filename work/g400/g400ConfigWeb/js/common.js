
function myAlert(myTitle, myMessage, myWidth, myHeight) {
    if (myTitle == null)
        myTitle = "提示";
    if (myWidth == 0)
        myWidth = 400;
    if (myHeight == 0)
        myHeight = 250;

    Ext.Msg.show({
        title: myTitle,
        msg: myMessage,
        width: myWidth,
        height: myHeight,
        icon: Ext.MessageBox.INFO,
        buttons: {
            yes: true
        }
    });
};

function button_hover_function() {
    $('#btLog').hover(function () {
        $(this).attr('class', 'btLoginOver');
    }, function () {
        $(this).attr('class', 'btLoginNor');
    });
    $("#btLog").mousedown(function () {
        $(this).attr('class', 'btLoginDown');
    });
    $(".mainButton").hover(function () {

        $(this).css({ 'color': '#0036FF', 'background-image': 'url(../images/button/btn_over.png)' });
    }, function () {
        $(this).css({ 'color': '#205699', 'background-image': 'url(../images/button/btn.png)' });
    });
    $(".mainButton").mousedown(function () {
        $(this).css({ 'color': '#0036FF', 'background-image': 'url(../images/button/btn_press.png)' });
    });
};

