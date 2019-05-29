# Find Your Seat (mobile app)
Here you can find the complete code of our mobile application. We use Ionic framework to devolop it, so it is compatible both with iOS and Android.

![alt text][AppTitle]


## Install Ionic Framework

To compile and run the application you need to install Angular (cause Ionic is based on it) and Ionic.
We recommend to follow the guides of the official website:
+ [Angular Installation](https://angular.io/guide/setup-local)
+ [Ionic Installation](https://ionicframework.com/docs/installation/cli)

![alt_text][AngularLogo]
![alt_text][IonicLogo]

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

## Add the key

If you didn't create the application on [The Things Network](https://www.thethingsnetwork.org), follow the steps described in the [Hackster Blog Post](https://www.hackster.io/andreanapoletani/find-your-seat-on-the-regional-train-using-your-app-525f37) (Section 2.1).

Now, take the Application Access Key (you can find it on the bottom of the overview page of your application) and the URL of the Data Storage integration. To retrieve the second one go to the 'Integration' page on your TTN app, select the Data Storage app and click on 'go to platform'. Cluck on one of the 3 APIs available and then click 'Try it out!', now you should see a field called 'requested URL'.

Open the file *mobile_app/src/app/train/train.component.ts* and find the method **getSeats()**.
You need to modify the first two variable initialized on this method:
```
const url = 'your-data-storage-url';
const headers = { 'Accept':'application/json','Authorization':'ttn-application-access-key'};
```
Instead of *your-data-storage-url* insert your Data Storage URL, and instead of *ttn-application-access-key* the Application Access key (ex: ttn-account-v2.<alphanumeric-sequence>).


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

![alt_text][AppLogo]


[AngularLogo]:https://d2eip9sf3oo6c2.cloudfront.net/tags/images/000/000/002/square_256/angularjs.png "Angular logo"
[IonicLogo]:https://static1.squarespace.com/static/56b8dfcf62cd94ec072ddb33/5b84f5704ae23726e2dcdd8e/5b84f5b4575d1fec274e92ad/1547134408986/112731_logo_512x512.png "Ionic logo"
[AppLogo]:https://i.imgur.com/KocQ02o.png "App Logo"
[AppTitle]:https://i.imgur.com/EGU1RpW.png "App title"
