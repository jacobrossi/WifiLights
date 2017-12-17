var express = require('express');
var storage = require('node-persist');
var router = express.Router();
var leds = require('../leds');

/* GET home page. */
router.get('/', function(req, res, next) {
    var tStart = new Date(leds.startTime);
    var tStop = new Date(leds.stopTime);
    res.render('index', {
        title: 'Christmas Lights',
        currentPattern: leds.pattern,
        brightness: leds.brightness,
        startTime: tStart.getHours().toString().padStart(2,"0") + ":" + tStart.getMinutes().toString().padStart(2,"0"),
        stopTime: tStop.getHours().toString().padStart(2,"0") + ":" + tStop.getMinutes().toString().padStart(2,"0"),
        color1: "#" + leds.color1.toString(16).padStart(6,"0"),
        color2: "#" + leds.color2.toString(16).padStart(6,"0"),
        helpers: {
        checked: function(id) {
            if (id==leds.pattern) {
            return 'checked';
            }
        },
        notchecked: function(id) {
            if (id!=leds.pattern) {
            return 'checked';
            }
        }
    }});
});

module.exports = router;
