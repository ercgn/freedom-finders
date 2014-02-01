package com.example.freedomfinders;

import android.app.Activity;
import android.database.Cursor;
import android.os.Bundle;
import android.provider.CalendarContract;
import android.util.SparseBooleanArray;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.Button;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;

public class ChooseCalendars extends Activity implements OnClickListener{
	private Cursor mCursor = null;
	private static final String[] COLS = new String[]
    		{ CalendarContract.Calendars.NAME};
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_choose_calendars);
        
        Button b = (Button)this.findViewById(R.id.getTimesButton);
        ListView listView = (ListView)this.findViewById(R.id.calendarList);
        b.setOnClickListener(this);
        mCursor = getContentResolver().query(CalendarContract.Events.CONTENT_URI, COLS, null, null, null);
        ListAdapter adapter = new SimpleCursorAdapter(this,
                R.layout.row, mCursor, COLS, new int[] { R.id.editText1 });
        listView.setAdapter(adapter);
        
        listView.setChoiceMode(ListView.CHOICE_MODE_MULTIPLE);

        listView.setItemsCanFocus(false);
        
        listView.setOnItemClickListener(new OnItemClickListener() {
        	void onItemClick(AdapterView<?> adapter, View arg1, int arg2, long arg3) {
        		SparseBooleanArray sp = getListView().getCheckedItemPositions();
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
		// TODO Auto-generated method stub
		// calls C program and executes and gets array yay!
	}
    
}
