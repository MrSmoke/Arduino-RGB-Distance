//pins
#define PIN_TRIGGER 9
#define PIN_ECHO 10

#define PIN_RED 1
#define PIN_GREEN 2
#define PIN_BLUE 3

//max distance in cm
#define MAX_DISTANCE 30

// what to do when the distance is greater than MAX_DISTANCE
// 0 = off
// 1 = last known colour
#define MAX_DISTANCE_COLOUR_MODE 0

//offsets
#define GREEN_OFFSET 0.5
#define BLUE_OFFSET -0.5
#define RED_OFFSET 0

int const rgbOff[3] = {0, 0, 0};

void setup()
{
    pinMode(PIN_TRIGGER, OUTPUT);
    pinMode(PIN_ECHO, INPUT);

    pinMode(PIN_RED, OUTPUT);
    pinMode(PIN_GREEN, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);
}

void loop()
{
    double distance = getDistance();

    //if the distance is within our max, calculate our values
    if (distance < MAX_DISTANCE)
    {
        int rgb[3] = {0, 0, 0};
        double distancePercent = distance / MAX_DISTANCE;
        calculateRGB(distancePercent, rgb);
        setRGB(rgb);
    }
    else
    {
        switch(MAX_DISTANCE_COLOUR_MODE)
        {
            case 0:
                setRGB(rgbOff);
                break;
            case 1:
                break;
        }
    }
}

void setRGB(const int rgb[])
{
    analogWrite(PIN_RED, rgb[0]);
    analogWrite(PIN_GREEN, rgb[1]);
    analogWrite(PIN_BLUE, rgb[2]);
}

//get the distance in cm
double getDistance()
{
    digitalWrite(PIN_TRIGGER, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIGGER, LOW);

    long duration = pulseIn(PIN_ECHO, HIGH);

    //convert to cm
    return duration * 0.034 / 2;
}

void calculateRGB(double distancePercent, int rgb[])
{
    method_simpleFade(distancePercent, rgb);
}

//basic triangle wave
int triangle(int position, int amplitude, int period)
{
    return ((double)amplitude / period) * (period - abs(abs(position) % (2 * period) - period));
}

//triangle wave with a single period
int triangle2(int position, int amplitude)
{
    int value = 0;

    if (position < amplitude)
    {
        value = position;
    }
    else
    {
        value = amplitude - (position - amplitude);
    }

    if (value < 0)
        return 0;

    return value;
}

//Methods

//some fluid tweening type deal
void method_tweeny(double distancePercent, int rgb[])
{
    int maxSteps = 255 * 2;

    int r = triangle((distancePercent + RED_OFFSET) * maxSteps, 255, maxSteps);
    int g = triangle((distancePercent + GREEN_OFFSET) * maxSteps, 255, maxSteps);
    int b = triangle((distancePercent + BLUE_OFFSET) * maxSteps, 255, maxSteps);

    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
}

//basic fading between colours
void method_simpleFade(double distancePercent, int rgb[])
{
    int maxSteps = 255 * 2;

    int r = triangle2((distancePercent + RED_OFFSET) * maxSteps, 255);
    int g = triangle2((distancePercent + GREEN_OFFSET) * maxSteps, 255);
    int b = triangle2((distancePercent + BLUE_OFFSET) * maxSteps, 255);

    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
}