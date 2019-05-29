# Find Your Seat (mobile app)
Here you can find the complete code of our mobile application. We use Ionic framework to devolop it, so it is compatible both with iOS and Android.

![alt text][AngularLogo]
![alt_text][IonicLogo]
![alt_text][AppLogo]

[AngularLogo]:https://d2eip9sf3oo6c2.cloudfront.net/tags/images/000/000/002/square_256/angularjs.png "Angular logo"
[IonicLogo]:https://static1.squarespace.com/static/56b8dfcf62cd94ec072ddb33/5b84f5704ae23726e2dcdd8e/5b84f5b4575d1fec274e92ad/1547134408986/112731_logo_512x512.png "Ionic logo"
[AppLogo]:https://imgur.com/CA6L7tO "App Logo"
## Install Ionic Framework

To compile and run the application you need to install Angular (cause Ionic is based on it) and Ionic.
We recommend to follow the guides of the official website:
+ [Angular Installation](https://angular.io/guide/setup-local)
+ [Ionic Installation](https://ionicframework.com/docs/installation/cli)

## Install dependencies

Now you have all the necessary to run our application.
So clone the git repo, navigate to mobile_app folder:
```
cd findyourseat/mobile_app
```
and then to install the dependeciens run:
```
npm i
```
Now you shuold see a new directory called node_modules.

### Run the app

To run the application on your localhost type the command:
```
ionic serve -c
```

### Run on your mobile

To run on your mobile you must have Android Studio or Xcode.
We recommend to use the integration with [Cordova](https://cordova.apache.org).
Follow this steps to run the app on your mobile:
+ [iOS device](https://ionicframework.com/docs/building/ios)
+ [Android device](https://ionicframework.com/docs/building/android)

