var express = require('express');
var storage = require('node-persist');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
    var tStart = new Date(storage.getItemSync('startTime'));
    var tStop = new Date(storage.getItemSync('stopTime'));
    var currentPattern = storage.getItemSync('currentPattern');
    res.render('index', {
        title: 'Christmas Lights',
        currentPattern: currentPattern,
        brightness: storage.getItemSync('brightness'),
        startTime: (tStart.getHours()=="0"?"00":tStart.getHours()) + ":" + tStart.getMinutes(),
        stopTime: (tStop.getHours()=="0"?"00":tStop.getHours()) + ":" + tStop.getMinutes(),
        color1: "#" + storage.getItemSync('color1').toString(16).padStart(6,"0"),
        color2: "#" + storage.getItemSync('color2').toString(16).padStart(6,"0"),
        helpers: {
        checked: function(id) {
            if (id==currentPattern) {
            return 'checked';
            }
        },
        notchecked: function(id) {
            if (id!=currentPattern) {
            return 'checked';
            }
        }
    }});
});

module.exports = router;
