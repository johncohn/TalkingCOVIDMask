## Talking COVID Mask
Talking COVID Mask - making people smile even when they don't always want to

![Here's me and my mask](/images/me.png)

# Basic idea
With everyone wearing masks I found it to be difficult to tell when someone was talking… Plus people needed a laugh so I created this simple animated face mask that moves when I talk. I’ve seen others on the web… I think what makes this one perhaps unique is it uses a little bit of adaptive averaging to adjust the audio range to keep the lips moving even as the speaker’s volume changes over time and situation. I’ve been wearing this to lots of zoom meetings and it creates lots of smiles… Something much-needed these days. Hope you enjoy it !
Eager to see how people improve on the design

Ps. I based the code on a design that my son Max and I did for a Red Bull challenge several years ago where we put the same kind of talking apparatus on the grill of a 6 ton surplus army truck that we use at burning man.  (see https://youtu.be/CejMA55O8L0)

# Parts:
* 1 Particle Photon   https://www.sparkfun.com/products/13764  (can easily be adapted to any Arduino compatible device if you don’t need wifi programability)
* 80 pixels of 144 LED/m addressable Neopixel strip e.g. https://www.adafruit.com/product/1506
* 1 3-pin Electret microphone  eg. https://www.sparkfun.com/products/12758 
* Thin Cloth facemask. Optimally translucent matte black 
* USB serial cable
* USB battery pack
* Wire, tape, hot glue
* the  Code   – tested on a particle PHOTON  which you can find in this repo


![here's the schematic](/images/schematic.png)

# Build
I developed this on a particle photon. It’s an Arduino compatible platform that has a nice web-based IDE that links to the chip via Wi-Fi. I have this set up so that it optionally looks for Wi-Fi… If found it will hook you up so that you can reflash the device wirelessly. If no Wi-Fi is found it just proceeds. The code is attached . The design could be easily adapted to any Arduino compatible device in a few minutes by taking out the particle specific stuff. I put in compile flags to allow that and got it to compile on any Arduino – but have not tested it. Shouldn’t be hard

The lights are 80 Neopixels  from a 144 LED/M strip such as these  .The LEDs Are arranged in eight vertical strips that are 10 LEDs each. They are soldered together and alternating directions to make an 8 x 10 matrix. I then hot glued the pack of the strips to a thin piece of mylar to hold them together. Note: I have experimented with flexible neopixel matrices like these but found that they were not flexible enough to be comfortable in a mask. Remember that the LED VCC needs to be wired directly to the USB VCC as the photon has a  3.3V and can’t supply the  voltage or source the current needed for  LED’s . I know it’s not completely kosher to run the new pixels data input  3.3 V but it works.

![](/images/glue.png)

![](/images/glued.png)

I used a 3 pin electret microphone with amplifier . I used  just the module, but most folks sell them as part of a breakout board like this. Either will work. Note there are a few constants in the photon sketch you may need to adjust to work with the output levels of your mic. Position the mic so it will be roughly over your mouth

![](/images/back.png)

Our wonderful friend Gretchen made the mask for me. We did lots of experimenting here to find the right color and weight of the fabric and led colors. We found that sheer black fabric and pink color for the LEDS  gave the best contrast in high light conditions. NOTE: even though I settled on a single uniform color, you can experiment with colors and color effects. I’m interested to see what others do with the expressiveness of other colors

![](/images/mask.png)

![](/images/maskclosed.png)

![](/images/maskopened.png)

![](/images/masksmile.png)

# Deploy
If you’re using a particle photon…  Go into the [particle IDE](https://build.particle.io/build/new) create a new sketch and copy [this code](/jc_mask_V10.ino) in. You must also include the libraries for the sparkintervaltimer and the neomatrix into the sketch.  From that point you should be able to deploy on the photon

Enjoy !
-jc
