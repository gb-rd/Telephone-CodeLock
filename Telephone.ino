#include <EEPROM.h>

const int pulse_move = 3;    // Disk move in
const int pulse_nums = 4;    // Disk pulse in
const int dynamic = 5;       // Phone dynamic out
const int relay_pin = 6;     // Relay out
const int ringer_1 = 7;      // Ringer coil 1 out
const int ringer_2 = 8;      // Ringer coil 2 out
const int linetime = 30000;  // Online wait time

int setted_password[4] = {2,2,2,2};      // Setted password here
int typed_password[4] = {0,0,0,0};       // Inputted password here

int digit_count = 0;
int i = 0;

int pulse_reading;
int pulse_moving;

boolean lastButtonState = LOW;   // the previous reading from the input pin
boolean buttonState;             // the current reading from the input pin
boolean digit_ready = 1;            // input digit counted
boolean pass_passed = 1;           // password compared
boolean pass_change = 0;         // password change mode enable

long timer;

long lastDebounceTime = 0;   // the last time the output pin was toggled
long debounceDelay = 30;     // the debounce time; increase if the output flickers

void setup() 
{
  pinMode(pulse_nums, INPUT_PULLUP);    // scheme with no external resistors
  pinMode(pulse_move, INPUT_PULLUP);    // scheme with no external resistors
  pinMode(relay_pin, OUTPUT);           // scheme with transistor, diode, relay
  pinMode(ringer_1, OUTPUT);            // scheme with transistor, diode, relay
  pinMode(ringer_2, OUTPUT);            // scheme with transistor, diode, relay
  
  pulse_moving = digitalRead( pulse_move );
  
  while (( pulse_moving == LOW ))      // change pass if disk moved
  {
    if (( millis() > linetime ) && ( pass_change == 0 ))
    {
      melody( 3, 2, 500 );      // Answer
      pass_change = 1;
    }
    else pulse_moving = digitalRead( pulse_move );
  }
  
  timer = ( millis() + linetime );

  while ( millis() < timer )                        // password input and compare
  {
    if ( i < 4 ) 
    {
      pulse_input();
    }
    else if ( i = 3 )
    {
      pass_passed = 0;
      password_compare( pass_change );
    }
  }
  
  melody( 7, 1, 500 );      // No tone
  melody( 2, 4, 500 );      // Busy
}

void loop()
{
}

void pulse_input()
{
  pulse_reading = digitalRead( pulse_nums );
  pulse_moving = digitalRead( pulse_move );
  
  if ( pulse_moving == LOW )
  {
    melody( 7, 1, 0 );      // No tone
    if ( pulse_reading != lastButtonState ) 
    {
      lastDebounceTime = millis();
    }
    if (( millis() - lastDebounceTime ) > debounceDelay )
    {
      if ( pulse_reading != buttonState )
      {
        buttonState = pulse_reading;
        if ( buttonState == HIGH ) 
        {
          digit_count++;
          digit_ready = 0;
          melody( 6, 1, 0 );      // Tone
          melody( 7, 1, 1 );      // No tone
        }
      }
    }
    lastButtonState = pulse_reading;
    timer = ( millis() + linetime );          // timer +
  }
  else
  {
    melody( 6, 1, 0 );      // Tone
    if ( digit_ready == 0 )
    {
      typed_password[i] = digit_count;
      i++;
      digit_ready = 1;
      digit_count = 0;
    }
  }  
}

void password_compare( boolean x )
{
  delay( 400 );
  melody( 7, 1, 0 );      // No tone
  switch ( x )
  {
    case 0:      // password compare
    {
      if ( pass_passed == 0 )
      {
        i = 0;
        for ( int j = 0; j < 4; j++ )      // read password from eeprom
        {
          setted_password[j] = EEPROM.read(j);
        }
        if (( typed_password[0] == setted_password[0] ) &&
           ( typed_password[1] == setted_password[1] ) &&
           ( typed_password[2] == setted_password[2] ) &&
           ( typed_password[3] == setted_password[3] ))
        {
          melody( 4, 2, 500 );      // Ring
          melody( 5, 12, 000 );      // Ding-Ding
          //melody( 3, 4, 1000 );      // Answer
          digitalWrite(relay_pin, 1);
          melody( 2, 1, 500 );      // Busy
          melody( 2, 1, 500 );      // Busy
          melody( 2, 1, 500 );      // Busy
          melody( 2, 1, 500 );      // Busy
          digitalWrite(relay_pin, 0);
          //melody( 6, 1, 1000 );      // Tone
          //melody( 7, 1, 1000 );      // No tone
          timer = millis();          // timer -
        }
        else
        {
          melody( 1, 2, 1500 );      // Not available
          melody( 2, 4, 500 );      // Busy
          delay( 1000 );
          timer = ( millis() + linetime );          // timer +
        }
        for ( int j = 0; j < 4; j++ ) typed_password[j] = 0;
        pass_passed = 1;
      }
    }
    case 1:      // password change
    {
      if ( pass_passed == 0 )
      {
        i = 0;
        for ( int j = 0; j < 4; j++ )
        {
          EEPROM.write(j, typed_password[j]);
          typed_password[j] = 0;
        }
        pass_passed = 1;
        pass_change = 0;
        melody( 3, 4, 500 );      // Answer
        timer = ( millis() + linetime );          // timer +
      }
    }
  }
}  

void melody( int x, int n, int t )
{
  delay( t );
  switch( x )
  {
    case 1:      // Not available
    {
      for ( int j = 0; j < n; j++ )
      {
        tone( dynamic, 950 );
        delay( 330 );
        tone( dynamic, 1400 );
        delay( 330 );
        tone( dynamic, 1800 );
        delay( 330 );
        noTone( dynamic );
        delay( 1000 );
      }
      break;
    }
    case 2:      // Busy
    {
      for ( int j = 0; j < n; j++ )
      {
        tone( dynamic, 425 );
        delay( 300 );
        noTone( dynamic );
        delay( 300 );
      }
      break;
    }
    case 3:      // Answer
    {
      for ( int j = 0; j < n; j++ )
      {
        tone( dynamic, 950 );
        delay( 150 );
        noTone( dynamic );
        delay( 150 );
      }
      break;
    }
    case 4:      // Ring
    {
      for ( int j = 0; j < n; j++ )
      {
        tone( dynamic, 425 );
        delay( 1000 );
        noTone( dynamic ); 
        delay( 3000 );
      }
      break;
    }
    case 5:      // Ding-Ding
    {
      for ( int j = 0; j < n; j++ )
      {
        digitalWrite(ringer_1, HIGH);
        delay( 25 );
        digitalWrite(ringer_1, LOW);
        delay( 1 );
        digitalWrite(ringer_2, HIGH);
        delay( 25 );
        digitalWrite(ringer_2, LOW);
        delay( 1 );
      }
      break;
    }
    case 6:      // Tone
    {
      tone( dynamic, 425 );
      break;
    }
    case 7:      // No tone
    {
      noTone( dynamic );
      break;
    }
  }
}
