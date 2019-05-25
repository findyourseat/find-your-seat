import { Component , OnInit } from '@angular/core';
import { Train } from '../train';
import { TrainService } from '../train.service';

@Component({
    selector: 'app-home',
    templateUrl: 'home.page.html',
    styleUrls: ['home.page.scss'],
})

export class HomePage implements OnInit {
    trains: Train[];
    condition = true;
    searchRes: Train[];

    constructor( private trainService: TrainService) {}

    ngOnInit() {
        this.getTrains();
    }

    getTrains(): void {
        this.trainService.getTrains().
        subscribe(trains => this.trains = trains);
    }

    search(id: string): void {
        if (id == '') {
            this.condition = true;
            this.searchRes = [];
            return;
        }
        this.condition = false;
        this.trainService.searchTrains(id).subscribe(trains => this.searchRes = trains);
    }

    cancelSearch(): void {
        this.condition = true;
        this.searchRes = [];
    }
}
