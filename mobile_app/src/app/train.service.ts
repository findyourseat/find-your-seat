import { Injectable } from '@angular/core';
import { Train } from './train';
import { Seat } from './seat';
import { Observable, of } from 'rxjs';
import { TRAINS } from './mock-trains';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { catchError, map, tap } from 'rxjs/operators';

import { HTTP } from '@ionic-native/http/ngx';
import {parseJson} from "@angular-devkit/core";


@Injectable({
    providedIn: 'root'
})

export class TrainService {
	// MARK: Properties	
	// Url to ttn api
    private seatUrl = 'https://28282828.data.thethingsnetwork.org/api/v2/query/02';  
	// header
	header = new HttpHeaders().set('Accept','application/json').append('Authorization','key ttn-account-v2.8JdJdrJG3YO2xss2lZonJWdnCJM66iNPiip4FC9pH3E');

	// MARK: Constructor
    constructor(private http: HTTP) { }

	// MARK: Methods
	/*
	// Return array of seats
    async getSeats(): Promise<JSON>{
		try {
            const url = 'https://28282828.data.thethingsnetwork.org/api/v2/query/02';
            const params = {};
            const headers = { 'Accept':'application/json','Authorization':'key ttn-account-v2.8JdJdrJG3YO2xss2lZonJWdnCJM66iNPiip4FC9pH3E'};

            const response = await this.http.get(url, params, headers);

		    console.log(response.status);
		    console.log(JSON.parse(response.data)); 
		    console.log(response.headers);

            return parseJson(response;
		} catch (error) {
		    console.error(error.status);
		    console.error(error.error); 
		    console.error(error.headers);
		}

		
    	// return this.http.get<Seat[]>(this.seatUrl, {headers: this.header}).pipe(catchError(this.handleError<Seat[]>('getSeats', [])));
    }
	*/
	// Returns all the trains
    getTrains(): Observable<Train[]> {
        return of(TRAINS);
    }

	// Return a train with specific id
    getTrain(id: string): Observable<Train> {
		let trains: Train[] = TRAINS;
		for(let train of trains) {
			if(train.id == id)	{	return of(train)	}
		}
		return of();
    }

    // Return trains whose name contains search term 
    searchTrains(term: string): Observable<Train[]> {
        if (!term.trim()) {
            // if not search term, return empty hero array.
            return of([]);
        }
		let trains: Train[] = TRAINS;
        let regexp = new RegExp(term, "gmi");
        return of(trains.filter(train => train.id.search(regexp) != -1));
    }
	
	// MARK: Private Functions
	
    private handleError<T>(operation = 'operation', result?: T) {
        return (error: any): Observable<T> => {
            // TODO: send the error to remote logging infrastructure
            console.error(error); // log to console instead
            // Let the app keep running by returning an empty result.
            return of(result as T);
        };
    }
}
