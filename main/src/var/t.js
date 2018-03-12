	function te(){

	a=prompt("a = ",a)
	if (a==null) {return} else {a=eval(a)}
	b=prompt("b = ",b)
	if (b==null) {return} else {b=eval(b)}

	del=a*a+b*b;

	x2=(a+Math.sqrt(del))/2;
	x=Math.sqrt(x2);y2=b*b/4/x2;
	y=b/2/x;

	x$=" = "+x.toString();if(x==Math.floor(x)) {x$=""}
	y$=" = "+y.toString();if(y==Math.floor(y)) {y$=""}

	sp=sgn(b);
	signe="x et y de même signe";if(sp==-1){signe="x et y de signes contraires"}
	alert("les racines carrées sont ±(x + iy) avec :"+"\n"+"x^2 = "+frac(x2)+"\n"+"y^2 = "+frac(y2)+"\n"+signe)
	alert("les racines carrées sont ±(x + iy) avec :"+"\n"+"x = "+frac(x)+x$+"\n"+"y = "+frac(y)+y$)

	}