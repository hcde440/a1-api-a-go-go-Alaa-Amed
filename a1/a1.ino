
// The Stupid Donald Trump Quote + News Tool You Never Know You Wanted!
// This tool utilizes the Tronald Dump API (https://docs.tronalddump.io/#introduction ) & the News API (https://newsapi.org/docs) 
// For the News API, you must get an  API key and attach it to the request

// Wi-Fi network, password, & API keys have been omitted from the code

#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266HTTPClient.h>  // Include this library, which provides methods to send HTTP requests.
#include <ArduinoJson.h>        //provides the ability to parse and construct JSON objects


const char* ssid = "University of Washington";  // The SSID (name) of the Wi-Fi network I am connected to 
const char* pass = ""; // The password of the Wi-Fi network
String key = ""; // Key for News API

String name = "";     // Will be used in the API call as the query parameter 
String input = "";    // Saves the input of the user from the serial monitor in the form of a string
String person = "";   // The name of the person to be searched (Ted, Hillary, or Joe)

typedef struct { // here we create a new data type definition, a box to hold other data types
  String val;
} Quote;     //then we give our new data structure a name so we can use it in our code

Quote said;        //we have created a Quote type, but not an instance of that type,
                   //so we create the variable 'said' of type Quote

typedef struct { // here we create a new data type definition, a box to hold other data types
  String author;
  String title;
  String description;

} News;            

News pub;          //we have created a News type, but not an instance of that type,
                   //so we create the variable 'pub' of type News

                   
void setup() { // Code runs only once or whenever we restart the board
  Serial.begin(115200); // Start the Serial communication to send messages to the computer
  delay(10); // Wait 1/10 of a second 
  Serial.print("This board is running: "); // print this message 
  Serial.println(F(__FILE__)); // print this message to the serial monitor
  Serial.print("Compiled: "); // print this message to the serial monitor
  Serial.println(F(__DATE__ " " __TIME__)); // print this message to the serial monitor
  
  Serial.print("Connecting to "); Serial.println(ssid); // print this message to the serial monitor + Wi-Fi network name

  WiFi.mode(WIFI_STA);    // Switch Wi-Fi mode
  WiFi.begin(ssid, pass); // Connect to the network

  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(500); // Wait 1/2 a second
    Serial.print("."); // Print loading dots to indicate an action is in progress
  }
  
  Serial.println(); Serial.println("WiFi connected"); Serial.println(); // if connected to the Wi-Fi, print this message 
  Serial.print("Your ESP has been assigned the internal IP address "); // print this message to the serial monitor
  Serial.println(WiFi.localIP()); // Print IP address to the serial monitor
  Serial.println("");
  Serial.println("--------------------------------------------------------------------------");   // Message greeting the user and asking them for input. 
  Serial.println("Hello! Welcome to stupid quotes by the one and only Donald Trump."); 
  Serial.println("With this tool you can look up Donald Trump quotes about certain people");
  Serial.println("--------------------------------------------------------------------------"); 
  Serial.println("");  
  Serial.println("For a quote about Joe Scarborough, the host of Morning Joe, ---->         |  enter 1");  
  Serial.println("For a quote about Ted Cruz, the Senator for Texas,          ---->         |  enter 2");
  Serial.println("For a quote about Hillary Clinton, Democratic Partys nominee              |"); 
  Serial.println("for President of the United States in the 2016 election,    ---->         |  enter 3");
   
  while (Serial.available()==0)  {               
    
  }
  
  input=Serial.readString();  //Reading the Input string from Serial port.
  int number = input.toInt(); // Casting the input to int  

 // Performing a comprising (I don't do error handling)
  if (number == 1) {  // If user input is "1"
    name = "joe";     // Make query paramter = Joe, which will lead to a Ted Cruz quote
    person = "Joe";
  } else if ( number == 2) {  // If user input is "2"
    name = "chris";   // Make query paramter = chris, which will lead to a Ted Cruz quote
    person = "Ted";
  } else {                // If user input is "3"
    name = "mexico";      // Make query paramter = mexico, which will lead to a Hillary quote
    person = "Hillary";
  }

  Serial.println("");
  dumbQuote(name, person); // Function call: go run the code in dumbQuote, accepts to parameters: name & person
  getHeadline();           // Function call: go run the code in getHeadline

  delay(1000);

  // Print source, headline, and description of article to the serial monitor
  Serial.println("--------------------------------------------------------------------------"); 
  Serial.println("");
  Serial.println("Now here is the latest headline from the BBC to blance out the stupidity you just read");
  Serial.println("Author: " + pub.author);
  Serial.println("Title:  " + pub.title);
  Serial.println("description: " + pub.description);
  

}

 void loop() {
  // we don't want to repeat anything so leave the loop empty 
}


void dumbQuote(String name, String person) {   // function called dumbQuote that provides Donald Trump Quotes
  HTTPClient theClient;  // Use HttpClient object to send requests
  //Serial.println("Making HTTP request"); // print this message to the serial monitor
  theClient.begin("http://api.tronalddump.io/search/quote?query="+ name); // API call with search query attached
  int httpCode = theClient.GET();
  if (httpCode > 0) { // if we get something back
    if (httpCode == HTTP_CODE_OK) {
      String payload = theClient.getString();
      DynamicJsonBuffer jsonBuffer; //  Dynamic Json buffer is allocated on the heap and grows automaticallyis 
      // it is also the entry point for using the library: it handles the memory management and calls the parser
      JsonObject& root = jsonBuffer.parseObject(payload);
      
      if (!root.success()) { // Test if parsing succeeds.
        Serial.println("parseObject() failed in getMet()."); // if parsing doesn't successed, print that to serial monitor 
        return;
      }
       said.val = root["_embedded"]["quotes"][0]["value"].as<String>();            // Parsing data. Find "_embedded", in its vlaue find "quotes". Quotes' value is a list 
                                                                                   // Go to the first item in the list, which had an index of 0. look for "value" and get its value 
                                                                                   // and save it as a string. Every other parsing is similar -- no need to comment 
       Serial.println("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");                                           
       Serial.println("\\\\\\ Here is a quote about " + person + "\" "+said.val+ "\"" +"\\\\\\");
       Serial.println("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");
    }
  }else {
    Serial.println("Something went wrong with connecting to the endpoint in getMet()."); // if we were not, for some reason, able to receive responses, then print this tp dserial monitor 
  }
}


void getHeadline() {   // function called getHeadline that provides BBC headlines  
  HTTPClient theClient;  // Use HttpClient object to send requests
  //Serial.println("Making HTTP request"); // print this message to the serial monitor
  theClient.begin("http://newsapi.org/v2/top-headlines?sources=bbc-news&apiKey="+ key); //API call with key
  int httpCode = theClient.GET();

  if (httpCode > 0) { // if we get something back
    if (httpCode == 200) { // and it's equal to 200
      //Serial.println("Received HTTP payload."); // then print this 
      DynamicJsonBuffer jsonBuffer;//  Dynamic Json buffer is allocated on the heap and grows automaticallyis 
      // it is also the entry point for using the library: it handles the memory management and calls the parser
      String payload = theClient.getString();
      //Serial.println("Parsing..."); // print this message to the serial monitor
      JsonObject& root = jsonBuffer.parse(payload);

      // Test if parsing succeeds.
      if (!root.success()) { // If parsing doesn't successed 
        Serial.println("parseObject() failed"); // Print this to serial monitor 
        Serial.println(payload); // Print the actual information or message in transmitted data, as opposed to automatically generated metadata.
        return;
      }

      pub.author = root["articles"][0]["author"].as<String>();           
      pub.title = root["articles"][0]["title"].as<String>();              
      pub.description = root["articles"][0]["description"].as<String>();  

    } else {
      Serial.println("Something went wrong with connecting to the endpoint."); // if we were not, for some reason, able to receive responses, then print this tp dserial monitor 
    }
  }
}
