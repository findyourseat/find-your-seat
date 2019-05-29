import { Component, OnInit , Input, ViewChild} from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { Location } from '@angular/common';
import { IonSegment } from '@ionic/angular';

import { Train } from '../train';
import { Seat } from '../seat';
import {Elem} from "../elem";
import { TrainService } from '../train.service';


import { HTTP } from '@ionic-native/http/ngx';


@Component({
    selector: 'app-train',
    templateUrl: './train.component.html',
    styleUrls: ['./train.component.scss'],
})
export class TrainComponent implements OnInit {
    @ViewChild(IonSegment) segment: IonSegment;

    @Input() train: Train;
    seats: Seat[];
    seat1: Seat;
    seat2: Seat;
    temp: number;
    response: Elem[];
    demo = false;
    oneFloor = false;
    twoFloor = false;
    segmentValue = 1;
    timeLeft = 10;
    interval;
    id;
	
    segmentChanged(event) {
        this.segmentValue = event.detail.value;
    }

    constructor(private route: ActivatedRoute ,
                private trainService: TrainService,
                private location: Location, private http: HTTP) {
    }

    ngOnInit() {
        this.getTrain();
        this.checkFloor();
		if (this.demo) {
			this.getSeats();
            this.startTimer();
		}

    }

    startTimer() {
        this.interval = setInterval(() => {
            if(this.timeLeft > 0) {
                this.timeLeft--;
            } else {
                this.timeLeft = 10;
                this.getSeats();
            }
        }, 1000);
    }


    async getSeats() {		
    	const url = 'your-data-storage-url';
	    const headers = { 'Accept':'application/json','Authorization':'ttn-application-access-key'};
	    const params = {};
	    

	    await this.http.get(url, params, headers).
			then(response => this.response = JSON.parse(response.data)).catch(err => this.seats = err);

	    this.seats = this.response.filter(elem => elem.temp == null);
        this.seat1 = this.seats.filter( seat => seat.seat_id == 1).pop();
        this.seat2 = this.seats.filter( seat => seat.seat_id == 2).pop();
	    this.response = this.response.filter(elem => elem.temp != null);
	    this.temp = this.response.pop().temp;
    }

    getTrain(): void {
        this.id = this.route.snapshot.paramMap.get('id');
        this.trainService.getTrain(this.id).subscribe(train => this.train = train);
    }

    goBack(): void {
        this.location.back();
    }

    checkFloor(): void {
        const regexp1 = new RegExp('RE1', 'gmi');
        const regexp2 = new RegExp('re20', 'gmi');
        const regexp3 = new RegExp('RE2', 'gmi');
        const regexp4 = new RegExp('RE3', 'gmi');
        if (this.id == 'RE11') {
            this.demo = true;
        } else if (this.id.search(regexp1) != -1 || this.id.search(regexp2) != -1){
            this.oneFloor = true;
        } else {
            this.twoFloor = true;
        }
    }
}
