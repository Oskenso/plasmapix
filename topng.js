var fs = require('fs');
var readline = require('readline');
var tty = require('tty');

var PNG = require('pngjs').PNG;

//2: rgb, 6: rgba
var png = new PNG({colorType: 6});

if ( ! tty.isatty(0)) {

	var rl = readline.createInterface({
		input: process.stdin,
		output: process.stdout,
		terminal: false
	});

	rl.on('line', function(line) {
		var obj = JSON.parse(line);
		png.width = obj.width;
		png.height = obj.height;
		
		var tData = new Uint32Array(png.width * png.height * 4);
		for (var i = 0; i < (png.width * png.height); i++) {
			tData[(i*4) + 0] = (obj.data[i] >> 24) & 0xFF;
			tData[(i*4) + 1] = (obj.data[i] >> 16) & 0xFF;
			tData[(i*4) + 2] = (obj.data[i] >> 8) & 0xFF;
			tData[(i*4) + 3] = obj.data[i] & 0xFF;
		}

		//console.log(tData);
		png.data = tData;

		png.pack().pipe(
			fs.createWriteStream('out.png')
		);
	});
}

/*
png.data = image.data;
png.width = 0;
png.height = 0;
png.pack().pipe(
	fs.createWriteStream('out.png')
);
*/
