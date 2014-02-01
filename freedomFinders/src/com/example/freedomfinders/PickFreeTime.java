package com.example.freedomfinders;

import java.util.ArrayList;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;

public class PickFreeTime extends Activity implements OnItemClickListener {
	
	private ArrayList<event> freeEvents;
	private ListView listView;
	private ArrayAdapter adapter;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_pick_free_time);
		
		listView = (ListView) findViewById(R.id.listView1);
		
		freeEvents = getIntent().getExtras().get(ChooseCalendars.FREE_TIMES);
		
		adapter = new ArrayAdapter(this, android.R.layout.simple_list_item_1, freeEvents);
		
		listView.setAdapter(adapter);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.pick_free_time, menu);
		return true;
	}

	@Override
	public void onItemClick(AdapterView<?> arg0, View arg1, int arg2, long arg3) {
		// TODO Auto-generated method stub
		
	}

}
