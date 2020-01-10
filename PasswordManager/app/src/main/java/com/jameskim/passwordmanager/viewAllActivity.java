package com.jameskim.passwordmanager;

import androidx.appcompat.app.AppCompatActivity;
import android.database.Cursor;
import android.os.Bundle;
import android.widget.ListView;
import android.widget.Toast;
import java.util.ArrayList;

public class viewAllActivity extends AppCompatActivity {

    DatabaseHelper db;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_view_all);

        db = new DatabaseHelper(this);
        ListView viewAllListView = findViewById(R.id.viewAllListView);

        ArrayList<Account> al = new ArrayList<>();
        Cursor data = db.getListContents();

        if( data.getCount() == 0) Toast.makeText(this, "Empty", Toast.LENGTH_SHORT).show();
        else{
            while(data.moveToNext()){
                String page = data.getString(1);
                String account = data.getString(2);
                String password = data.getString(3);

                Account newAccount = new Account(page,account,password);
                al.add(newAccount);

                //ListAdapter listAdapter = new ArrayAdapter<>(this,android.R.layout.simple_list_item_1,al);
                //viewAllListView.setAdapter(listAdapter);
            }
        }

        // Apply adapter
        AccountListAdapter adapter = new AccountListAdapter(this, R.layout.adapter_view_layout, al);
        viewAllListView.setAdapter(adapter);
    }// end of onCreate
}
