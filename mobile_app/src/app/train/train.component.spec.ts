import { CUSTOM_ELEMENTS_SCHEMA } from '@angular/core';
import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { TrainComponent } from './train.component';

describe('TrainComponent', () => {
  let component: TrainComponent;
  let fixture: ComponentFixture<TrainComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ TrainComponent ],
      schemas: [CUSTOM_ELEMENTS_SCHEMA],
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(TrainComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
