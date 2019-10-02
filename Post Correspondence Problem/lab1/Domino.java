package lab1;

public class Domino{
	public String name = "";
	public String top = "";
	public String bot = "";

	public Domino(){
	}

	public Domino(String name, String top, String bot){
		this.name = name;
		this.top = top;
		this.bot = bot;
	}

	// order vulnerable 
	public Domino add (Domino anotherDomino){
		Domino newDomino = new Domino();
		newDomino.name = this.name +" " + anotherDomino.name;
		newDomino.top = this.top + anotherDomino.top;
		newDomino.bot = this.bot + anotherDomino.bot;
		return newDomino;
	}
	
	// return 0 if not valid 
	// return 1 if valid state 
	// return 2 if goal state
	public int validate(){
		if( this.top.equals(this.bot)) return 2;
		else{
			// "aabbb" 
			// "aabc"
			for ( int i = 0; i < Math.min(this.top.length(), this.bot.length()) ; i ++){
				if(this.top.charAt(i) != this.bot.charAt(i)) return 0;
			}
			return 1; 
		}
	}
}