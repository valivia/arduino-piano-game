const int servo = 5;
const int buzzer = 6;
const int button = 4;

const int led_red = 9;
const int led_green = 10;
const int led_blue = 11;

#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494

#define NOTE_C5 523
#define NOTE_D5 587
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_G5 784
#define NOTE_A5 880
#define NOTE_B5 988

int notemap[][3] = {
    {14, NOTE_C4, false},
    {15, NOTE_D4, false},
    {16, NOTE_E4, false},
    {17, NOTE_F4, false},
    {18, NOTE_G4, false},
    {19, NOTE_C5, false},
    {8, NOTE_B4, false},
};

const int ladder[][2] = {
    {NOTE_C4, 125},
    {NOTE_D4, 125},
    {NOTE_E4, 125},
    {NOTE_F4, 125},
    {NOTE_G4, 125},
    {NOTE_A4, 125},
    {NOTE_B4, 125},
};

int song[][2] = {
    {NOTE_G4, 125},
    {NOTE_G4, 125},
    {NOTE_F4, 125},
    {NOTE_G4, 250},
    {NOTE_D4, 250},
    {NOTE_D4, 125},
    {NOTE_G4, 125},
    {NOTE_C5, 125},
    {NOTE_B4, 125},
    {NOTE_G4, 250},
};

void setup()
{
  pinMode(button, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(led_red, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(led_blue, OUTPUT);

  for (int i = 0; i < 7; i++)
  {
    pinMode(notemap[i][0], INPUT_PULLUP);
  }
}

static bool played = false;
static bool freeplay = false;
static bool buttonPressed = false;
static int noteIndex = 0;

void loop()
{
  if (!played && !freeplay)
    playSong();

  if (!digitalRead(button) && !buttonPressed)
  {
    buttonEvent();
  }

  if (digitalRead(button))
    buttonPressed = false;

  for (int i = 0; i < 7; i++)
  {
    if (notemap[i][2])
    {
      if (!digitalRead(notemap[i][0]))
      {
        notemap[i][2] = false;
        noTone(buzzer);
      }
      else
        return;
    }
  }

  for (int i = 0; i < 7; i++)
  {
    if (digitalRead(notemap[i][0]))
    {
      notemap[i][2] = true;
      playNote(i);
    }
  }
}

void playNote(int x)
{
  tone(buzzer, notemap[x][1], 0);

  if (freeplay)
    return;

  // Wrong note.
  if (notemap[x][1] != song[noteIndex][0])
  {
    noteIndex = 0;
    rgbLed(255, 0, 0);
    delay(20);
    rgbLed(0, 0, 0);
    return;
  }
  else
  {
    // Finished puzzle.
    if (noteIndex == (sizeof(song) / sizeof(*song)) - 1)
    {
      rgbLed(0, 255, 0);
      open();
      freeplay = true;
      noteIndex = 0;
      return;
    }

    noteIndex += 1;
  }
}

void rgbLed(int r, int g, int b)
{
  digitalWrite(led_red, r);
  digitalWrite(led_green, g);
  digitalWrite(led_blue, b);
}

void buttonEvent()
{
  buttonPressed = true;
  // restart song.
  rgbLed(0, 255, 255);
  delay(1000);
  if (digitalRead(button))
  {
    rgbLed(0, 0, 0);
    played = false;
    return;
  }

  // switch mode.
  delay(1000);
  freeplay = !freeplay;
  noteIndex = 0;

  if (freeplay)
  {
    rgbLed(255, 0, 255);
  }
  else
    rgbLed(0, 0, 0);

  delay(1000);
  return;
}

void playSong()
{
  rgbLed(0, 0, 255);
  const int totalNotes = (sizeof(song) / sizeof(*song));
  for (int i = 0; i < totalNotes; i++)
  {

    if (!digitalRead(button))
    {
      played = true;
      rgbLed(0, 0, 0);
      delay(9000);
      return;
    }

    int currentNote = song[i][0];
    float wait = song[i][1] / .5;
    if (currentNote != 0)
    {
      tone(buzzer, currentNote, wait);
    }
    else
    {
      noTone(buzzer);
    }
    delay(wait);
  }
  rgbLed(0, 0, 0);

  played = true;
}

void open()
{
}