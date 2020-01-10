package com.jameskim.passwordmanager;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;

import java.util.Set;

public class  MainActivity extends AppCompatActivity {


    Button addAccountBtn, viewAllBtn;
    private static LinearLayout mainActLinearLayout;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        addAccountBtn = findViewById(R.id.mainAddAccountBtn);
        viewAllBtn = findViewById(R.id.mainViewAll);
        mainActLinearLayout = findViewById(R.id.mainLinearLayout);

        // restore page buttons created from previous sessions
        SharedPreferences settings = PreferenceManager.getDefaultSharedPreferences(this);
        Set<String> pageTitleSet = settings.getStringSet("page_button_key", null);

        if (!pageTitleSet.isEmpty()) {
            for (String pageTitleText : pageTitleSet) {
                Button pageBtn = new Button(this);
                pageBtn.setText(pageTitleText);
                mainActLinearLayout.addView(pageBtn);

                pageBtn.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        Intent intent = new Intent(getApplicationContext(), CreateAccountActivity.class);
                        startActivity(intent);
                    }
                });
            }
        }

        addAccountBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this,CreateAccountActivity.class);
                startActivity(intent);
            }
        });

        viewAllBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent (MainActivity.this, viewAllActivity.class);
                startActivity(intent);
            }
        });

    }

    public static LinearLayout getMainActLinearLayout() {
        return mainActLinearLayout;
    }
}
