# Plasmapix

Generate plasma images

``Usage: plasmapix width height time [density] [weight]``
Outputs image structure in JSON
```sh
npm install
make
./plasmapix 240 320 0 | node topng.js
```
![Plasma image result](https://oskenso.com/plasma-out.png)

### JSON output
```sh
plasmapix 2 2 0
```
```js
{
	width: 100,
	height: 100,
	data: {
		"width":2,
		"height":2,
		"data":[
			2146242815,
			1442589951,
			2029065471,
			1646459391
		]
	}
}```


None of this could have been done without the info provided by
[Bidouille.org](http://www.bidouille.org/prog/plasma)
