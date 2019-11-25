/*
	Name:       WEBElements.ino
	Created:	04.05.2019
	Author:     borstel
	Inhalt:		Webseiten mit den Web Elementen
*/
const char HTTP_TITLE_LABEL[] PROGMEM = "<div class='l lt'><label>{v}</label><hr /></div>";
const char HTTP_TITLE_LABEL_1[] PROGMEM = "<br><div class='l'><label>{vs}</label><hr /></div>";
const char HTTP_BUTTON_SAVE[] PROGMEM = "<div><button name='btnSave' value='1' type='submit'>Speichern</button></div>";
const char HTTP_BUTTON_HOME[] PROGMEM = "<div><input class='lnkbtn' type='button' value='Home' onclick=\"window.location.href='/'\" /></div>";
const char HTTP_BUTTON_LED[] PROGMEM = "<div><input class='lnkbtn' type='button' value='{led}' onclick=\"window.location.href='/LED'\" /></div>";
const char HTTP_BUTTON_confAll[] PROGMEM = "<div><input class='lnkbtn' type='button' value='Konfiguration' onclick=\"window.location.href='/configAllg'\" /></div>";
const char HTTP_BUTTON_CONFIG_CUxD[] PROGMEM = "<div><input class='lnkbtn' type='button' value='Konfiguration CUxD' onclick=\"window.location.href='/configCUxD'\" /></div>";
const char HTTP_BUTTON_CONFIG_WIFI[] PROGMEM = "<div><input class='lnkbtn' type='button' value='Konfiguration WIFI' onclick=\"window.location.href='/bootConfigMode'\" /></div>";
const char HTTP_BUTTON_LED_LED[] PROGMEM = "<div><input class='lnkbtn' type='button' value='{led}' onclick=\"window.location.href='/LED'\" /></div>";
const char HTTP_BUTTON_NEWFW[] PROGMEM = "<div><input class='fwbtn' id='fwbtn' type='button' value='Neue Firmware verf&uuml;gbar' onclick=\"window.open('{fwurl}')\" /></div><div><input class='fwbtn' id='fwbtnupdt' type='button' value='Firmwaredatei einspielen' onclick=\"window.location.href='/update'\" /></div>";
const char HTTP_BUTTON_confLED[] PROGMEM = "<div><input class='lnkbtn' type='button' value='{text}' onclick=\"window.location.href='/configLED'\" /></div>";
const char HTTP_LABEL_STATUS[] PROGMEM = "<div class='l c'>{sl}</div>";
const char HTTP_LABEL_FW[] PROGMEM = "<div class='l c k'><label>Firmware: {fw}</label></div>";

const char HTTP_CONF_CCUIP[] PROGMEM = "<div><label>{st}:</label></div><div><input type='text' id='ccuip' name='ccuip' pattern='((^|\\.)((25[0-5])|(2[0-4]\\d)|(1\\d\\d)|([1-9]?\\d))){4}$' maxlength=16 placeholder='{st}' value='{ccuip}'></div>";

//neu mit replace
// Anzeigen
const char HTTP_ANZEIGE_RADIO[] PROGMEM = "{bez}<input type='radio' id='{id}' name='{name}' value='{val}' {ch} onclick='return false'>";
const char HTTP_ANZEIGE_TXT[] PROGMEM = "<div id='{id}'><pre>{bez1}{bez2}{bez3}</pre></div>";
// eingabe masken
// const char HTTP_CONF_TXT[] PROGMEM = "<div><pre>{bez}<input type='text' id='{id}' name='{name}' pattern='{pat}' placeholder='{pla}' value='{val}'></pre></div>";
const char HTTP_CONF_TXT[] PROGMEM = "<div>{bez}<input type='text' id='{id}' name='{name}' pattern='{pat}' placeholder='{pla}' value='{val}'></div>";
const char HTTP_CONF_LEDCOLOR[] PROGMEM = "{bez}<input type='color' id='{id}' name='{name}' value='{val}'>";
const char HTTP_CONF_DATE[] PROGMEM = "{bez}<input type='DATE' id='{id}' name='{name}' placeholder='{pla}' value='{val}'>";
const char HTTP_CONF_TIME[] PROGMEM = "{bez}<input type='time' id='{id}' name='{name}' placeholder='{pla}' value='{val}'>";
const char HTTP_CONF_RADIO[] PROGMEM = "{bez}<input type='radio' id='{id}' name='{name}' value='{val}' {ch}>";
const char HTTP_CONF_CHKBox[] PROGMEM = "{bez}<input type='checkbox' id='{id}' name='{name}' value='{val}' {ch}>";

const char HTTP_CONF_TZLIST[] PROGMEM = "<div><pre>{bez}<input list='{list}' name='{name}' value='{val}'> \
<datalist id='{id}'> \
<option DayTime=0 value=-12>(GMT-12:00) International Date Line West </option> \
<option DayTime=0 value=-11>(GMT-11:00) Midway Island, Samoa </option> \
<option DayTime=0 value=-10>(GMT-10:00) Hawaii </option> \
<option DayTime=1 value=-9>(GMT-09:00) Alaska </option> \
<option DayTime=1 value=-8>(GMT-08:00) Pacific Time (US Canada) </option> \
<option DayTime=1 value=-8>(GMT-08:00) Tijuana, Baja California </option> \
<option DayTime=0 value=-7>(GMT-07:00) Arizona </option> \
<option DayTime=1 value=-7>(GMT-07:00) Chihuahua, La Paz, Mazatlan </option> \
<option DayTime=1 value=-7>(GMT-07:00) Mountain Time (US Canada) </option> \
<option DayTime=0 value=-6>(GMT-06:00) Central America </option> \
<option DayTime=1 value=-6>(GMT-06:00) Central Time (US Canada) </option> \
<option DayTime=1 value=-6>(GMT-06:00) Guadalajara, Mexico City, Monterrey </option> \
<option DayTime=0 value=-6>(GMT-06:00) Saskatchewan </option> \
<option DayTime=0 value=-5>(GMT-05:00) Bogota, Lima, Quito, Rio Branco </option> \
<option DayTime=1 value=-5>(GMT-05:00) Eastern Time (US Canada) </option> \
<option DayTime=1 value=-5>(GMT-05:00) Indiana (East) </option> \
<option DayTime=1 value=-4>(GMT-04:00) Atlantic Time (Canada) </option> \
<option DayTime=0 value=-4>(GMT-04:00) Caracas, La Paz </option> \
<option DayTime=0 value=-4>(GMT-04:00) Manaus </option> \
<option DayTime=1 value=-4>(GMT-04:00) Santiago </option> \
<option DayTime=1 value=-3.5>(GMT-03:30) Newfoundland </option> \
<option DayTime=1 value=-3>(GMT-03:00) Brasilia </option> \
<option DayTime=0 value=-3>(GMT-03:00) Buenos Aires, Georgetown </option> \
<option DayTime=1 value=-3>(GMT-03:00) Greenland </option> \
<option DayTime=1 value=-3>(GMT-03:00) Montevideo </option> \
<option DayTime=1 value=-2>(GMT-02:00) Mid-Atlantic </option> \
<option DayTime=0 value=-1>(GMT-01:00) Cape Verde Is. </option> \
<option DayTime=1 value=-1>(GMT-01:00) Azores </option> \
<option DayTime=0 value=0>(GMT+00:00) Casablanca, Monrovia, Reykjavik </option> \
<option DayTime=1 value=0>(GMT+00:00) Greenwich Mean Time : Dublin, Edinburgh, Lisbon, London </option> \
<option DayTime=1 value=1>(GMT+01:00) Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna </option> \
<option DayTime=1 value=1>(GMT+01:00) Belgrade, Bratislava, Budapest, Ljubljana, Prague </option> \
<option DayTime=1 value=1>(GMT+01:00) Brussels, Copenhagen, Madrid, Paris </option> \
<option DayTime=1 value=1>(GMT+01:00) Sarajevo, Skopje, Warsaw, Zagreb </option> \
<option DayTime=1 value=1>(GMT+01:00) West Central Africa </option> \
<option DayTime=1 value=2>(GMT+02:00) Amman </option> \
<option DayTime=1 value=2>(GMT+02:00) Athens, Bucharest, Istanbul </option> \
<option DayTime=1 value=2>(GMT+02:00) Beirut </option> \
<option DayTime=1 value=2>(GMT+02:00) Cairo </option> \
<option DayTime=0 value=2>(GMT+02:00) Harare, Pretoria </option> \
<option DayTime=1 value=2>(GMT+02:00) Helsinki, Kyiv, Riga, Sofia, Tallinn, Vilnius </option> \
<option DayTime=1 value=2>(GMT+02:00) Jerusalem </option> \
<option DayTime=1 value=2>(GMT+02:00) Minsk </option> \
<option DayTime=1 value=2>(GMT+02:00) Windhoek </option> \
<option DayTime=0 value=3>(GMT+03:00) Kuwait, Riyadh, Baghdad </option> \
<option DayTime=1 value=3>(GMT+03:00) Moscow, St. Petersburg, Volgograd </option> \
<option DayTime=0 value=3>(GMT+03:00) Nairobi </option> \
<option DayTime=0 value=3>(GMT+03:00) Tbilisi </option> \
<option DayTime=1 value=3.5>(GMT+03:30) Tehran </option> \
<option DayTime=0 value=4>(GMT+04:00) Abu Dhabi, Muscat </option> \
<option DayTime=1 value=4>(GMT+04:00) Baku </option> \
<option DayTime=1 value=4>(GMT+04:00) Yerevan </option> \
<option DayTime=0 value=4.5>(GMT+04:30) Kabul </option> \
<option DayTime=1 value=5>(GMT+05:00) Yekaterinburg </option> \
<option DayTime=0 value=5>(GMT+05:00) Islamabad, Karachi, Tashkent </option> \
<option DayTime=0 value=5.5>(GMT+05:30) Sri Jayawardenapura </option> \
<option DayTime=0 value=5.5>(GMT+05:30) Chennai, Kolkata, Mumbai, New Delhi </option> \
<option DayTime=0 value=5.75>(GMT+05:45) Kathmandu </option> \
<option DayTime=1 value=6>(GMT+06:00) Almaty, Novosibirsk </option> \
<option DayTime=0 value=6>(GMT+06:00) Astana, Dhaka </option> \
<option DayTime=0 value=6.5>(GMT+06:30) Yangon (Rangoon) </option> \
<option DayTime=0 value=7>(GMT+07:00) Bangkok, Hanoi, Jakarta </option> \
<option DayTime=1 value=7>(GMT+07:00) Krasnoyarsk </option> \
<option DayTime=0 value=8>(GMT+08:00) Beijing, Chongqing, Hong Kong, Urumqi </option> \
<option DayTime=0 value=8>(GMT+08:00) Kuala Lumpur, Singapore </option> \
<option DayTime=0 value=8>(GMT+08:00) Irkutsk, Ulaan Bataar </option> \
<option DayTime=0 value=8>(GMT+08:00) Perth </option> \
<option DayTime=0 value=8>(GMT+08:00) Taipei </option> \
<option DayTime=0 value=9>(GMT+09:00) Osaka, Sapporo, Tokyo </option> \
<option DayTime=0 value=9>(GMT+09:00) Seoul </option> \
<option DayTime=1 value=9>(GMT+09:00) Yakutsk </option> \
<option DayTime=0 value=9.5>(GMT+09:30) Adelaide </option> \
<option DayTime=0 value=9.5>(GMT+09:30) Darwin </option> \
<option DayTime=0 value=10>(GMT+10:00) Brisbane </option> \
<option DayTime=1 value=10>(GMT+10:00) Canberra, Melbourne, Sydney </option> \
<option DayTime=1 value=10>(GMT+10:00) Hobart </option> \
<option DayTime=0 value=10>(GMT+10:00) Guam, Port Moresby </option> \
<option DayTime=1 value=10>(GMT+10:00) Vladivostok </option> \
<option DayTime=1 value=11>(GMT+11:00) Magadan, Solomon Is., New Caledonia </option> \
<option DayTime=1 value=12>(GMT+12:00) Auckland, Wellington </option> \
<option DayTime=0 value=12>(GMT+12:00) Fiji, Kamchatka, Marshall Is. </option> \
<option DayTime=0 value=13>(GMT+13:00) Nuku'alofa </option> \
</datalist></pre></div>";