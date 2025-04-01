# DOSDEMO
A small collection of simple demo effects for DOS using mode 13h, written to be complied with [Open Watcom 1.9](https://github.com/open-watcom/open-watcom-1.9). 

## Building
With Open Watcom 1.9 installed, just run "wmake" to build all effects.

## Effects

### Fire
A simple fire effect with some horizontal offset on the seed lines to make it more interesting.

![fire_000](https://github.com/user-attachments/assets/8ff3ccd0-154b-4b8e-8a3e-e7aefc042f7b)

### Plasma
Classic plasma effect using a sine table and fixed point arithmetic.

![plasma_001](https://github.com/user-attachments/assets/75f64299-3c37-436c-a222-2e67bc256f9d)

### Wormhole
Palette rotation wormhole effect. It doesn't look very good right now as the bitmap was converted from a 640x480 BMP to a 320x200 PCX and a lot of artefacts were introduced.

![wormhole_000](https://github.com/user-attachments/assets/379eab68-937b-4176-85ce-3e3f3e764239)
