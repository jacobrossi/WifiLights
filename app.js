var express = require('express');
var path = require('path');
var favicon = require('serve-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var exphbs  = require('express-handlebars');
var storage = require('node-persist');

var index = require('./routes/index');
var api = require('./routes/api');

var app = express();

//Initialize storage
storage.initSync();
if(!storage.getItemSync('startTime')) {
    storage.setItemSync('startTime',new Date(2016,1,1,16,30,0,0));
}
if(typeof storage.getItemSync('stopTime')) {
    storage.setItemSync('stopTime',new Date(2016,1,1,0,15,0,0));
}
if(!storage.getItemSync('currentPattern')) {
    storage.setItemSync('currentPattern','1');
}
if(!storage.getItemSync('color1')) {
    storage.setItemSync('color1',1671168);
}
if(!storage.getItemSync('color2')) {
    storage.setItemSync('color2',65280); //Green
}
if(!storage.getItemSync('brightness')) {
    storage.setItemSync('brightness',30);
}

app.set('views', path.join(__dirname, 'views'));
app.engine('handlebars', exphbs({defaultLayout: 'main',
    layoutsDir: (!!process.env.PORT?'../views/layouts':'./views/layouts')
}));
app.set('view engine', 'handlebars');

app.locals.currentPattern = "1";

app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', index);
app.use('/api', api);
app.use('/api/.', api);
app.use(favicon(__dirname + '/public/images/favicon.ico'));

// catch 404 and forward to error handler
app.use(function(req, res, next) {
    var err = new Error('Not Found');
    err.status = 404;
    next(err);
});

// error handler
app.use(function(err, req, res, next) {
    // set locals, only providing error in development
    res.locals.message = err.message;
    res.locals.error = req.app.get('env') === 'development' ? err : {};

    // render the error page
    res.status(err.status || 500);
    res.render('error');
});

module.exports = app;
