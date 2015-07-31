var addon = require('./build/Release/addon');

addon.run(function(err, param) {
    console.log(arguments);
});

setInterval(function() {console.log(new Date);}, 5000);
