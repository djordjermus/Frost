using Frost.Net;
using Frost.Net.Models;

double start = Clock.Timestamp;
var random = new Frost.Net.Random();
while (true)
{
	HSLA hsla = new HSLA()
	{
		h = (float)((Clock.Timestamp - start) * Clock.Period % 1.0),
		s = 1.0f,
		l = 0.5f,
		a = 1.0f
	};
	HSVA hsva = new HSVA()
	{
		h = (float)((Clock.Timestamp - start) * Clock.Period % 1.0),
		s = 1.0f,
		v = 1.0f,
		a = 1.0f
	};
	CMYK cmyk = new CMYK()
	{
		c = (float)((Clock.Timestamp - start) * Clock.Period % 1.0),
		m = 1.0f,
		y = 0.5f,
		k = 1.0f
	};
	var c	= new Color(hsla);
	c		= new Color(hsva);
	c		= new Color(cmyk);

	var hdr = new HDRColor(hsla);
	hdr		= new HDRColor(hsva);
	hdr		= new HDRColor(cmyk);

	hdr		= new HDRColor(c);
	c		= new Color(hdr);

	hsla	= c.ToHsla();
	hsla	= hdr.ToHsla();
	hsva	= c.ToHsva();
	hsva	= hdr.ToHsva();
	cmyk	= c.ToCmyk();
	cmyk	= hdr.ToCmyk();

	Thread.Sleep(10);
}
