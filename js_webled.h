const char HTTP_WEBLEDSCRIPT[] PROGMEM = R"=====(

/*Aktualiesierung der ESP Temperatur auf html Seite */
setTimeout(function(){ refreshTemp(); }, 1000); 
function refreshTemp() { 
  var json_obj = JSON.parse(Get('/getTempJSON')); 
  document.getElementById('aTemp').innerHTML = json_obj.joTemp; 
  setTimeout(function(){ refreshTemp() }, 1000); 
}
)=====";
