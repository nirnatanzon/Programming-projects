/**
 * This class represents a Rent object
 */
public class Rent
{

    private String _name; 
    private Car _car;
    private Date _pickDate;
    private Date _returnDate;
    
    private int _price;
    private int _daysOfRent;
    
    private static final int TYPE_A_DAY_PRICE = 100; 
    private static final int TYPE_B_DAY_PRICE = 150; 
    private static final int TYPE_C_DAY_PRICE = 180; 
    private static final int TYPE_D_DAY_PRICE = 240; 
    
    private static final int TYPE_A_WEEK_PRICE = 630; 
    private static final int TYPE_B_WEEK_PRICE = 945; 
    private static final int TYPE_C_WEEK_PRICE = 1134; 
    private static final int TYPE_D_WEEK_PRICE = 1512; 
    private static final int WEEK_DAYS = 7; 

    
    private static final char TYPE_A = 'A';
    private static final char TYPE_B = 'B';
    private static final char TYPE_C = 'C';
    
   /**
      *Creates a new Rent object
        The return date must be at least one day after the pickup date, otherwise set it to one day after the pick up date  
      *
      *
      *@param name - the client's name
       *@param car - the rented car
       *@param pick - the pickup date
       *@param ret - the return date   
    */
    public Rent (String name, Car car, Date pick, Date ret){
        setName(name);
        setCar(car);
        this._pickDate = pick;
        
        if(ret.after(pick)){
            setReturnDate(ret);
        }else{
            setReturnDate(new Date(pick.tomorrow()));
        }
        
    }
    /**
      *Copy constructor
      *@param other - the rent to be copied
    */
    public Rent (Rent other){
        this._name = other._name;
        this._car = new Car (other._car);
        this._pickDate = new Date (other._pickDate);
        this._returnDate = new Date (other._returnDate);
    }
    /** Check if 2 rents are the same
      * @param other - the rent to compare this rent to
      * @return true if the rents are the same
     */ 
    public boolean equals (Rent other){
       if (this._car.equals(other._car) && this._pickDate.equals(other._pickDate)
       && this._returnDate.equals(other._returnDate) && this._name==other._name){
           return true;
       }
       return false;
    }
     /** Gets the car
      * return the car
     */ 
    public Car getCar(){
        return this._car;
    }
    /** Gets the name
      * return the name
     */ 
    public java.lang.String getName(){
        return this._name;
    }
     /** Gets the pick up date
      * return the pick up date
     */ 
    public Date getPickDate(){
         return this._pickDate;
    }
     /** Gets the return date
      * return the return date
     */ 
    public Date getReturnDate(){
        return this._returnDate;
    }
     /** Sets the rented car
      * @param car - the rented car (You can assume that car is not null)
     */
    public void setCar(Car car){
         this._car = car;
    }
     /** Sets the client name
      * @param name - the client name (You can assume that the name is a valid name)
     */
    public void setName(java.lang.String name){
         this._name = name;
    }
    /** Sets the pick up date
        The pick up date must be at least one day before the return date, otherwise - don't change the pick up date
      * @param pickDate - the pick up date (You can assume that pick up date is not null)
     */
    public void setPickDate(Date pickDate){
         if(pickDate.before(this._returnDate)){
             this._pickDate = pickDate;
        }
    }
    /** Sets the return date
    The return date must be at least one day after the pick up date, otherwise - don't change the return date
      * @param returnDate - the return date (You can assume that return date is not null)
     */ 
    public void setReturnDate(Date returnDate){
          if(returnDate.after(this._pickDate)){
            this._returnDate = returnDate;
          }
    }
    /** Returns a String that represents this rent
    *toString in class java.lang.Object 
    * @param String that represents this rent in the following format:
    Name:Rama From:30/10/2022 To:12/11/2022 Type:B Days:13 Price:1845   
     */ 
    public java.lang.String toString(){
       return ("Name:" + this._name + " From:" + this._pickDate + " To:" + this._returnDate + " Type:" + this._car.getType() + 
       " Days:" + howManyDays() + " Price:" + this.getPrice());  
    }
    /** Returns the number of rent days
    * return the number of rent days   
     */ 
    public int howManyDays(){
            return this._pickDate.difference(this._returnDate);
     }
     /** Returns the rent total price
    * return the rent total price   
     */ 
    public int getPrice(){
      int rentDays = this.howManyDays();
      int discountedWeeks = rentDays / WEEK_DAYS;
      int fullPriceDays = rentDays % WEEK_DAYS;
      if(this._car.getType() == TYPE_A){
          return ((discountedWeeks * TYPE_A_WEEK_PRICE) + (fullPriceDays * TYPE_A_DAY_PRICE));
      }else if(this._car.getType() == TYPE_B){
           return ((discountedWeeks * TYPE_B_WEEK_PRICE) + (fullPriceDays * TYPE_B_DAY_PRICE));
      }else if(this._car.getType() == TYPE_C){
         return ((discountedWeeks * TYPE_C_WEEK_PRICE) + (fullPriceDays * TYPE_C_DAY_PRICE));
      }
      return ((discountedWeeks * TYPE_D_WEEK_PRICE) + (fullPriceDays * TYPE_D_DAY_PRICE));
    }
    /** Try to upgrade the car to a better car
    If the given car is better than the current car of the rent, upgrade it and return the upgrade additional cost, otherwise - don't upgrade
      * @param car - the car to upgrade to
      * @return the upgrade cost
        */
    public int upgrade (Car newCar){
        this._price = getPrice();
        
        if(this._car.worse(newCar)){
            setCar(newCar);
            return Math.abs(this._price - getPrice());
        }
        return 0;
    }
    /** Check if there is a double listing of a rent for the same person and car with an overlap in the rental days
        If there is - return a new rent object with the unified dates, otherwise - return null. 
      * @param other - the other rent
      * @return the unified rent or null
        */
    public Rent overlap (Rent other){
       Rent overlapRent = new Rent(this);
       if(this._name.compareTo(other._name) == 0 && this._car.equals(other._car)){
          if(this._returnDate.after(other._returnDate)){
            overlapRent.setReturnDate(this._returnDate);
          }else overlapRent.setReturnDate(other._returnDate);
          if(this._pickDate.before(other._pickDate)){
              overlapRent.setPickDate(this._pickDate);
            }else overlapRent.setPickDate(other._pickDate);
          return overlapRent;
        }
        return null;
    }
    }
