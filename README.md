# VRChatOverlay

This project utilizes the **[OpenVR API/SDK](https://github.com/ValveSoftware/openvr)**, **[the Simple and Fast Multimedia Library](http://www.sfml-dev.org/)** and **[OpenGL](https://www.opengl.org/)**

Link against:  
- openvr_api.lib
- sfml-main-d.lib  
- sfml-system-d.lib  
- sfml-window-d.lib  
- sfml-graphics-d.lib  
- opengl32.lib  

Compiles neatly using Visual Studio 2015 (haven't tried GCC yet)  

## Some random thoughts:  

You can have relative positioned or absolute overlays.  
You can have overlays starting in a relative position and then move on their own (steam chat messages in HMD)  

You can have a dashboard overlay (see these black buttons in your steam menu while inside your HMD? yes you can have your own black button there with your own thumbnail (not yet it seems) and can have your own window/chat displayed).  
You can even enable the keyboard when using the overlay so you could write back from withing VR  

You could probably add a button/gesture to enabled/disable the overlay.  

You could probably have streaming video on the overlay (for people that want to watch netflix while in VR or something).  

## Known "bugs":  
Overlay flickers  
Program doesn't even connect to any chat service for now  

## Thoughts on architecture:  
Main overlay tool that just displays the overlay and allows to be configured (dashboard overlay, normal overlay; position absolute, relative, moving etc; size of the overlay, alpha value etc).  
Then 'plugins' for each chat service (IRC & Twitch, Google Hangouts, Skype(?), Discord, Hitbox, ...whatever).  

_Note to self: Learn how to format this shit..._  
