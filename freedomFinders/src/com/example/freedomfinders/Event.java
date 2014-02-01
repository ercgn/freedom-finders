package com.example.freedomfinders;

public class Event {
	public String start;
	public String end;
	public String rrule;
	
	public Event(String start, String end, String rrule) {
		this.start = start;
		this.end = end;
		this.rrule = rrule;
	}
	
	public Event(String start, String end) {
		this.start = start;
		this.end = end;
	}
	
	public Event() {}
	
	public Event[] findFreeTimes(Event[] events) {
		return null;
	}
}
