
/**
 * This class represents a Date object
 */

public class Date
{
    private int _day;
    private int _month;
    private int _year;
    
    private static final int DEFAULT_DAY = 1;
    private static final int DEFAULT_MONTH = 1;
    private static final int DEFAULT_YEAR = 2000;
    
    private static final int LONG_MONTH = 31;
    private static final int SHORT_MONTH = 30;
    private static final int FEBRUARY_LEAP = 29;
    private static final int FEBRUARY_NON_LEAP = 28;
    
    private static final int YEARSFERST = 1000;
    private static final int YEARSEND = 9999;
    
    private static final int MONTH1 = 1; 
    private static final int MONTH2 = 2;
    private static final int MONTH3 = 3;
    private static final int MONTH4 = 4; 
    private static final int MONTH5 = 5;
    private static final int MONTH6 = 6;
    private static final int MONTH7 = 7;
    private static final int MONTH8 = 8;
    private static final int MONTH9 = 9;
    private static final int MONTH10 = 10; 
    private static final int MONTH11 = 11;
    private static final int MONTH12 = 12; 
    
    private static final int DAY1= 1;
    private static final int DAY28= 28;
    private static final int DAY29= 29;
    private static final int DAY30 = 30;
    private static final int DAY31 = 31;
    
   /**
      * If the given date is valid - creates a new Date object, otherwise creates the date 1/1/2000
      *
      *
      *@param day - the day in the month (1-31)
      *@param month - the month in the year (1-12)
      *@param year - the year (4 digits)
    */
    public Date (int day, int month, int year){
        if(checkDate(day,month,year)){
            setDay(day);
            setMonth(month);
            setYear(year);
        }else{
            setDay(DEFAULT_DAY);
            setMonth(DEFAULT_MONTH);
            setYear(DEFAULT_YEAR);
        }
    }
    /**
      *Copy constructor
      *@param other - the date to be copied
    */
    public Date (Date other){
        this._day = other._day;
        this._month = other._month;
        this._year = other._year; 
    }
   /**
      *Set the day (only if date remains valid)
      *@param dayToSet - the day value to be set
    */
    public void setDay(int dayToSet){
        _day = dayToSet;
    }
    /**
      *Set the month (only if date remains valid)
      *@param monthToSet - the month value to be set
    */
    public void setMonth(int monthToSet){
        _month = monthToSet;
    }
    /**
      *Sets the year (only if date remains valid)
      *@param yearToSet - the year value to be set
    */
    public void setYear(int yearToSet){
        _year = yearToSet;
    }
    /**
      *Gets the day
      *return dayToSet - the day value to be set
    */
    public int getDay(){
        return this._day;
    }
    /**
      *Gets the month
      *return the month
    */
    public int getMonth(){
        return this._month;
    }
   /**
      *Gets the year
      *return the year
    */
    public int getYear(){
        return this._year;
    }
    
    /**This method gets the day, month and year as parameters.
    The method checks if the day is valid, by calaculating leap years and months with 31 OR 30 days
    and returns true if the day is valid. For invalid dates the method returns false.
    
    * Check if date is valid

    */
    private boolean checkDate(int day, int month, int year){
        if(year > YEARSEND || year < YEARSFERST )
        return false;
        if((month == MONTH1 || month == MONTH3 || month == MONTH5 || month == MONTH7 || month == MONTH8 || month == MONTH10 || month == MONTH12)
            && ((day>= DAY1) && (day <= DAY31))){
            return true;
        }
        else if((month == MONTH4 || month == MONTH6 || month == MONTH9 || month == MONTH11) && ((day >= DAY1) && (day <= DAY30))){
            return true;
        } 
        else if (month == MONTH2){
          // Calculation for leap years of February   
          if (((year % 4 == 0) && (year % 100!= 0)) || (year % 400 == 0) && (day >= DAY1) && (day <= DAY29)){
                return true;
            }
            else if((day >= DAY1) && (day <= DAY28)){
                return true;
            }
        }
        return false;   
    }
    /**
      *Returns a String that represents this date
      *toString in class java.lang.Object
      *return String that represents this date in the following format: day (2 digits) / month(2 digits) / year (4 digits) for example: 02/03/1998
    */
    public String toString(){
     return ( (_day<10 ? "0" : "") +  _day + "/" + (_month<10 ? "0" : "") + _month + "/" + _year);
    }
     /**
      *Check if 2 dates are the same
      *@param other - the date to compare this date to
      *return true if the dates are the same, otherwise false
    */
    public boolean equals (Date otherDate){
        if (otherDate._day == this._day && otherDate._month == this._month && otherDate._year == this._year)
       {
           return true;
       }
       return false;
    }
      /**
      *Check if this date is before other date
      *@param other - date to compare this date to
      *return true if this date is before other date, otherwise false
    */
    public boolean before (Date other){
        if( this._year < other._year ){
            return true;
        } else if( this._year > other._year ){
            return false;
        } else if( this._year == other._year){
            if( this._month < other._month ){
                return true;
            } else if( this._month > other._month ){
                return false;
            } else if( this._month == other._month ){
                if( this._day < other._day ){
                    return true;
                } else if( this._day > other._day ){
                    return false;
                }
            }
        }
        return false;
    }
     /**
      *Check if this date is after other date
      *@param other - date to compare this date to
      *return true if this date is after other date, otherwise false
    */
    public boolean after (Date other){
     return other.before(this);
    }
    /**
      *Calculates the difference in days between two dates
      *@param other - the date to calculate the difference between
      *return the number of days between the dates (non negative value)   
    */
    public int difference (Date other){
     return Math.abs(calculateDate(this._day,this._month, this._year) - calculateDate(other._day, other._month, other._year));
    }
   // computes the day number since the beginning of the Christian counting of years
   private int calculateDate ( int day, int month, int year){
         if (month < 3) {
             year--;
             month = month + 12;
         }
         return 365 * year + year/4 - year/100 + year/400 + ((month+1) * 306)/10 + (day - 62);
     }
     /**
      *Calculate the date of tomorrow
      *return the date of tomorrow   
    */
     public Date tomorrow(){
        Date tomorrowDate = new Date(this);
        
        if( this._month == MONTH1 || this._month == MONTH3 || this._month == MONTH5 || this._month == MONTH7 || this._month == MONTH8 || this._month == MONTH10){
             if (this._day == LONG_MONTH){
                tomorrowDate.setMonth(tomorrowDate._month + 1);
                tomorrowDate.setDay(1);
            }else{
                tomorrowDate.setDay(tomorrowDate._day + 1);
            }
         }else if( this._month == MONTH4 || this._month == MONTH6 || this._month == MONTH9 || this._month == MONTH11){
             if (this._day == SHORT_MONTH){
                tomorrowDate.setMonth(tomorrowDate._month + 1);
                tomorrowDate.setDay(DEFAULT_DAY);
            }else{
                tomorrowDate.setDay(tomorrowDate._day + 1);
            }
         }else if(this._month == MONTH12){
             if (this._day == LONG_MONTH){
                tomorrowDate.setDay(DEFAULT_DAY);
                tomorrowDate.setMonth(DEFAULT_MONTH);
                tomorrowDate.setYear(tomorrowDate._year + 1);
            }else{
                tomorrowDate.setDay(tomorrowDate._day + 1);
            }
            }else if(this._month == MONTH2){
                
                    if (this._day == FEBRUARY_LEAP || this._day == FEBRUARY_NON_LEAP){
                        tomorrowDate.setMonth(tomorrowDate._month + 1);
                        tomorrowDate.setDay(DEFAULT_DAY);
                    }else{
                            tomorrowDate.setDay(tomorrowDate._day + 1);
                    }
            }
            return tomorrowDate;   
     }
}
