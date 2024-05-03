
/**
 * This class represents a Car object
 */

public class Car
{
    private int _id;
    private char _type;
    private String _brand;
    private boolean _isManual;
    
    private static final int DEFAULT_PLATE = 9999999;
    private static final char DEFAULT_TYPE = 'A';
    private static final int ID1= 999999;
    private static final int ID2= 10000000;
    /**
      *Creates a new Car object
      *id should be a 7 digits number, otherwise set it to 9999999
      *type should be 'A','B','C' or 'D', otherwise set it to 'A'
      *
      *
      *@param id - the id of the car (7 digits number)
      *@param type - the type of the car ('A','B','C' or 'D')
      *@param brand - the car's brand
      *@param isManual - flag indicating if the car is manual
    */
    public Car(int id, char type, String brand, boolean isManual){
        setId(id);
        setType(type);
        setBrand(brand);
        setIsManual(isManual);
    }
    /** Copy constructor
     *@param other - the car to be copied
     */
    public Car(Car other){
        this._id = other._id;
        this._type = other._type;
        this._brand = other._brand;
        this._isManual = other._isManual;
    }
    
    //Setters
     /** Sets the id (only if the given id is valid)
      * @param id - the id value to be set
     */
    void setId (int id){
        if(ID1 < id && id < ID2){
            _id = id;
        }else{
            _id = DEFAULT_PLATE;
        }
    }
    /** Sets the type (only if the given type is valid)
      * @param type - the type value to be set
     */
    void setType (char type){
        if(type == 'A' || type == 'B' || type == 'C' || type == 'D'){
            _type = type;
        }else{
            _type = DEFAULT_TYPE;
        }
    }
    /** Sets the brand of the car
      * @param brand - the brand value to be set
     */
    void setBrand (String brand){
        _brand = brand;
    }
      /** Sets the isManual flag of the car
      * @param isManual - the isManual flag to be set
     */
    void setIsManual (boolean manual){
        _isManual = manual;
    }
    //Getters
      /** Gets the id
      * @return the id
     */
    public int getId (){
        return this._id;
    }
    /** Gets the type
      * @return the type
     */
    public char getType (){
        return this._type;
    }
      /** Gets the brand
      * @return the brand
     */ 
    public String getBrand (){
        return this._brand;
    }
      /** Gets the isManual flag
      * @return the isManual flag
     */ 
    public boolean isManual (){
        return this._isManual;
    }
     /** toString in class java.lang.Object
      * @return String that represents this car in the following format:
        id:1234567 type:B brand:Toyota gear:manual (or auto)
     */ 
    public String toString() {
        return ("id:" + this._id + " type:" + this._type + " brand:" + this._brand + " gear:" + (this._isManual ? "manual" : "auto"));
    }
     /** Check if two cars are the same
        Cars are considered the same if they have the same type, brand and gear
      * @param other - the car to compare this car to
      * @return true if the cars are the same, otherwise false
     */ 
    public boolean equals (Car other){
        if(this._type == other._type && this._brand.equals(other._brand) && this._isManual == other._isManual){
            return true;
        }else{
            return false;
        }
    }
     /** Check if this car is better than the other car
        A car is considered better than another car if its type is higher.
        If both cars have the same type, an automated car is better than a manual car.  
      * @param other - car to compare this car to
      * @return true if this car is better than the other car, otherwise false
     */ 
    public boolean better (Car other){
        if(this._type > other._type) {
         return true;
        }else if((this._type == other._type) && (!this._isManual) && (other._isManual)){
            return true;
        }
        return false;
    }
     /**Check if this car is worse than the other car  
      * @param other - car to compare this car to
      * @return true if this car is worse than the other car, otherwise false
     */ 
    public boolean worse (Car other){
        return other.better(this);
    }
}
