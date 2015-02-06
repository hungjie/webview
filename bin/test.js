/*
try{
	jsQObject.Sendtojs.connect(slotfrom);
}catch(e){
	alert(e);
}
object["cur_index"] = 0;
		object["cur_input"] = "";
		object["time"] = 1000;
		object["input_array"] = jsQObject.get_search_input_array();
		object["input_id"] = jsQObject.get_search_input_id();
*/

var main_script = [{"func":"scroll","parms":{"id":"kw"}},{"func":"move","parms":{"id":"kw"}},{"func":"lbclick","parms":{"id":"kw"}},{"func":"timerinputvalue","parms":{"cur_input":"", "time":1000, "input_array":["L","O"], "input_id":"kw"}},{"func":"move","parms":{"id":"su", "offset":{"left":5, "top":5}}}, {"func":"lbclick","parms":{"id":"su", "offset":{"left":5, "top":5}}}];

function func() {
    try {
        //var object = {intValue: 2};
        //var returnedObject = jsQObject.slotThatReturns(object);
        //alert(returnedObject.stringValue);
        dump_obj(jsQObject);
        jsQObject.Sendtojs.connect(slotfrom);
        jsQObject.slotThatEmitsSignal();
    }
    catch(e){
        alert(e);
    }
}

function dump_obj(myObject) {
  var s = "";
  for (var property in myObject) {
   s = s + "\n "+property +": " + myObject[property] ;
  }
  alert(s);
}

//factory_action define the flow

function lbclick(object)
{
	this.object = object;
	this.action = function()
	{
		//alert("test");
		//var left = this.object.left;
		//var top = this.object.top;
		
		var top = -1;
		var left = -1;
		
		var e = document.getElementById(this.object.id);
		if(e != undefined && e != null)
		{
			var top = getElementTop(e);
			var left = getElementLeft(e);
		}
		
		var offset_top = 0;
		var offset_left = 0;
		
		if(this.object.offset != undefined)
		{
			offset_top = this.object.offset.top;
			offset_left = this.object.offset.left;
		}
		var parms = {"left":left+offset_left, "top":top+offset_top};
		
		jsQObject.lbclick(parms);
	}
}

function mbroll(object)
{
	this.object = object;
	this.action = function()
	{
		var top = -1;
		var left = -1;
		
		var e = document.getElementById(this.object.id);
		if(e != undefined && e != null)
		{
			var top = getElementTop(e);
			var left = getElementLeft(e);
		}
		
		var ch = this.object.ch;
		
		var parms = {"left":left, "top":top, "ch":ch};
		
		jsQObject.mbroll(parms);
	}
}

function mbclick(object)
{
	this.object = object;
	this.action = function()
	{		
		var top = -1;
		var left = -1;
		
		var e = document.getElementById(this.object.id);
		if(e != undefined && e != null)
		{
			var top = getElementTop(e);
			var left = getElementLeft(e);
		}
		
		var offset_top = 0;
		var offset_left = 0;
		
		if(this.object.offset != undefined)
		{
			offset_top = this.object.offset.top;
			offset_left = this.object.offset.left;
		}
		var parms = {"left":left+offset_left, "top":top+offset_top};
		
		jsQObject.mbclick(parms);
	}
}

function move(object)
{
	this.object = object;
	this.action = function()
	{
		var e = document.getElementById(this.object.id);
		var top = getElementTop(e);
		var left = getElementLeft(e);
		
		var offset_top = 0;
		var offset_left = 0;
		
		if(this.object.offset != undefined)
		{
			offset_top = this.object.offset.top;
			offset_left = this.object.offset.left;
		}
		var parms = {"left":left+offset_left, "top":top+offset_top};
		
		jsQObject.move(parms);
	}
}

function scroll(object)
{
	this.object = object;
	this.action = function()
	{
		var e = document.getElementById(this.object.id);
		var sy = getElementTop(e);
		var sx = getElementLeft(e);
		
		var parms = {"left":sx, "top":sy};
		
		jsQObject.scroll(parms);
	}
}

function timerinputvalue(object)
{
	this.object = object;
	
	this.action = function(){
		jsQObject.timerInput(this.object);
	}
}

function switchtab(object)
{
	this.object = object;
	this.action = function(){
		var index = this.object.index;
		jsQObject.switchtab(index);
	}
}

function sleep(object)
{
	this.object = object;
	this.action = function(){
		var mtime = this.object.mtime;
		jsQObject.sleep(mtime);
	}
}

function factory(action)
{
	var o;
	
	if(main_script.length == 0 || main_script.length <= action)
		return o;
		
	var parms = main_script[action];
	
	if(parms.func == "lbclick")
	{
		o = new lbclick(parms.parms);
	}
	else if(parms.func == "move")
	{
		o = new move(parms.parms);
	}
	else if(parms.func == "scroll")
	{
		o = new scroll(parms.parms);
	}
	else if(parms.func == "timerinputvalue")
	{
		o = new timerinputvalue(parms.parms);
	}
	else if(parms.func == "switchtab")
	{
		o = new switchtab(parms.parms);
	}
	else if(parms.func == "sleep")
	{
		o = new sleep(parms.parms);
	}
	
	return o;
}

function factory_action(object) {
	//dump_obj(object);
	var step = object.signalsEmited;
	var a = factory(step);
	
	if(a == null || a == undefined)
	{
		return;
	}
		
	a.action();
}

function start()
{
	//alert(globalvar);
    //alert("start");
    // > a[href='http://big.taobao.com/?spm=1.7388829.a214tlt.19']
    /*
	var e = qt.jQuery("#OSC_Links");
    var top = e.offset().top;
    var left = e.offset().left;
	*/
	
	//var e = document.getElementById("HomeJobPanel");
	/*
	var es = document.getElementsByClassName("pic-link");
	alert(es.length);
	alert(jsQObject.isLoadFinished());
	return;
	*/
	
	var step = 0;
	jsQObject.Sendtojs.connect(factory_action);
	
	var parms = {"signalsEmited":step};
	factory_action(parms);
}

function getElementLeft(element)
{
    var actualLeft = element.offsetLeft;
    var current = element.offsetParent;
    while (current !== null){
    actualLeft += current.offsetLeft;
    current = current.offsetParent;
    }
    return actualLeft;
}

function getElementTop(element)
{
    var actualTop = element.offsetTop;
    var current = element.offsetParent;
    while (current !== null){
    actualTop += current.offsetTop;
    current = current.offsetParent;
    }
    return actualTop;
}

function getElementViewLeft(element){
    var actualLeft = element.offsetLeft;
    var current = element.offsetParent;
    while (current !== null){
    actualLeft += current.offsetLeft;
    current = current.offsetParent;
    }
    if (document.compatMode == "BackCompat"){
    var elementScrollLeft=document.body.scrollLeft;
    } else {
    var elementScrollLeft=document.documentElement.scrollLeft;
    }

    return actualLeft-elementScrollLeft;
}

function getElementViewTop(element){
    var actualTop = element.offsetTop;
    var current = element.offsetParent;
    while (current !== null){
    actualTop += current. offsetTop;
    current = current.offsetParent;
    }
     if (document.compatMode == "BackCompat"){
    var elementScrollTop=document.body.scrollTop;
    } else {
    var elementScrollTop=document.documentElement.scrollTop;
    }
    return actualTop-elementScrollTop;
}
