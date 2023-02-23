# ObserverPatterTest
### A very simple test with 2 circles for Observer Pattern, using Raylib

It is made up for 2 classes: *Observer* and *Subject*, located on the file Observer_Subject.h

Observer is the one that waits for the notification and needs to "subscribe" to the "notification feed", Subject is the one that notifies all the observer when something happens. 
This is done through a simple overriden function called **OnNotify**, so when its called, the Observer acts accordingly

#### This is a very simple and naive implementation, just wanted to see how it "looks"
