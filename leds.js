var storage = require('node-persist');
//LED DEFAULTS
var _startTime = new Date(2016,1,1,16,30,0,0), //4:30pm
_stopTime = new Date(2016,1,1,0,15,0,0), //12:15am
_color1 = 255, //Blue
_color2 = 65280,//Green
_brightness= 30, //30%
_pattern= '1' //Random

/* Notes about storage and performance:
    The 8266 microcontroller operates synchronously. This means the http requests to get the latest LED info are synchronous with writing to the LEDs -- in other words,
    the LED animations will drop frames while the WiFi/Http/JSON routine runs. So, to optimize performance on the server side, we use in-memory variables for all the
    LED settings (the led object above). But to ensure uptime and resiliancy, the settings are async written to disk via node-persist. This enables the code below to
    reinitialize the leds object from disk should node restart. Sync read/write from node-persist can be too slow for a good experience.
*/

var leds = {
    set startTime(val) {
        _startTime = val;
        storage.setItem('startTime',val);
        console.log('Set startTime to ' + val);
    },
    get startTime() {return _startTime},

    set stopTime(val) {
        _stopTime = val;
        storage.setItem('stopTime',val);
        console.log('Set stopTime to ' + val);
    },
    get stopTime() {return _stopTime},

    set color1(val) {
        _color1 = val;
        storage.setItem('color1',val);
        console.log('Set color1 to ' + val);
    },
    get color1() {return _color1},

    set color2(val) {
        _color2 = val;
        storage.setItem('color2',val);
        console.log('Set color2 to ' + val);
    },
    get color2() {return _color2},

    set brightness(val) {
        _brightness = val;
        storage.setItem('brightness',val);
        console.log('Set brightness to ' + val);
    },
    get brightness() {return _brightness},

    set pattern(val) {
        _pattern = val;
        storage.setItem('pattern',val);
        console.log('Set pattern to ' + val);
    },
    get pattern() {return _pattern}
};

//Load or init settings in persistent storage
console.log('Initalizing storage at ' + __dirname);
storage.init({dir: __dirname}).then(function() {
    return storage.getItem('startTime');
}).then(function(val) {
    if (typeof val != 'undefined')  {
        _startTime = val;
        console.log('Init startTime to ' + val);
    }
}).then(function() {
    return storage.getItem('stopTime');
}).then(function(val) {
    if (typeof val != 'undefined')  {
        _stopTime = val;
        console.log('Init stopTime to ' + val);
    }
}).then(function() {
    return storage.getItem('color1');
}).then(function(val) {
    if (typeof val != 'undefined')  {
        _color1 = val;
        console.log('Init color1 to ' + val);
    }
}).then(function() {
    return storage.getItem('color2');
}).then(function(val) {
    if (typeof val != 'undefined')  {
        _color2 = val;
        console.log('Init color2 to ' + val);
    }
}).then(function() {
    return storage.getItem('brightness');
}).then(function(val) {
    if (typeof val != 'undefined')  {
        _brightness = val;
        console.log('Init brightness to ' + val);
    }
}).then(function() {
    return storage.getItem('pattern');
}).then(function(val) {
    if (typeof val != 'undefined')  {
        _pattern = val;
        console.log('Init pattern to ' + val);
    }
});

module.exports = leds;