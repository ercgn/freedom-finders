package com.example.freedomfinders;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Color;
import android.os.Bundle;
import android.provider.CalendarContract;
import android.util.SparseBooleanArray;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;

public class ChooseCalendars extends Activity implements OnClickListener{
	
	public static String FREE_TIMES = "free_time_slots";
	private Cursor mCursor = null;
	private static final String[] COLS = new String[]
    		{ CalendarContract.Calendars.NAME, CalendarContract.Calendars._ID};
	private ListView listView;
	private Button b;
	private SparseBooleanArray sp;
	private ArrayAdapter adapter;
	private Map <String,String> calendars;
	ArrayList<String> selectedItems;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_choose_calendars);
        
        b = (Button)this.findViewById(R.id.getTimesButton);
        listView = (ListView)this.findViewById(R.id.calendarList);
        b.setOnClickListener(this);
        
        mCursor = getContentResolver().query(CalendarContract.Calendars.CONTENT_URI, COLS, null, null, null);
        
        calendars =  new HashMap<String,String>();
        
        if (mCursor.moveToFirst()) {
            do {
            	calendars.put(mCursor.getString(0), mCursor.getString(1));
            } while (mCursor.moveToNext());
        }
            
        adapter = new ArrayAdapter(this, android.R.layout.simple_list_item_1);
        adapter.addAll(calendars.keySet());
        listView.setAdapter(adapter);
        
        listView.setChoiceMode(ListView.CHOICE_MODE_MULTIPLE);

        listView.setItemsCanFocus(false);
        
        listView.setOnItemClickListener(new OnItemClickListener() {
        	public void onItemClick(AdapterView<?> adapter, View arg1, int arg2, long arg3) {
        		arg1.setBackgroundColor(Color.GREEN);
        		sp = listView.getCheckedItemPositions();
        		
        		selectedItems = new ArrayList<String>();
                for (int i = 0; i < sp.size(); i++) {
                    // Item position in adapter
                    int position = sp.keyAt(i);
                    // Add sport if it is checked i.e.) == TRUE!
                    if (sp.valueAt(i))
                        selectedItems.add(calendars.get(adapter.getItemAtPosition(position)));
                }
        	}
        });
        
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.choose_calendars, menu);
        // Possibly upload another calendar :<
        return true;
    }


	@Override
	public void onClick(View arg0) {
		// Get all events from selected calendars
		 String[] cols = new String[]
	    		{ CalendarContract.Events.CALENDAR_ID, CalendarContract.Events.DTSTART, CalendarContract.Events.DTEND};
		 mCursor = getContentResolver().query(CalendarContract.Events.CONTENT_URI, COLS, null, null, null);
		 event *events = new ArrayList<event>;
		 if (mCursor.moveToFirst()) {
	            do {
	            	if (selectedItems.contains(mCursor.getString(0))) {
	            		event e = new event();
	            		e->start = mCursor.getString(1);
	            		e->emd = mCursor.getString(2);
	            		events.add(e);
	            	}
	            } while (mCursor.moveToNext());
	        }
	          
		 Intent intent = new Intent();
		 intent.putExtra(FREE_TIMES, events);
	}
}
