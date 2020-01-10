package com.jameskim.passwordmanager;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.util.HashSet;

public class CreatePasswordActivity extends AppCompatActivity {

    EditText editText1, editText2;
    Button btn;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_create_password);

        editText1 = (EditText) findViewById(R.id.createPasswordEditText1);
        editText2 = (EditText) findViewById(R.id.createPassWordEditText2);
        btn = (Button) findViewById(R.id.createPasswordButton);

        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String firstInput = editText1.getText().toString();
                String secondInput = editText2.getText().toString();

                if ( firstInput.equals("") || secondInput.equals("")){
                    Toast.makeText(CreatePasswordActivity.this,"No Password Entered!", Toast.LENGTH_SHORT).show();
                }
                else {
                    if (firstInput.equals(secondInput)){
                        SharedPreferences settings = PreferenceManager.getDefaultSharedPreferences(CreatePasswordActivity.this);
                        SharedPreferences.Editor editor = settings.edit();
                        editor.putString("password", firstInput).putStringSet("page_button_key", new HashSet<String>()).apply();

//                        editor.putString("password",firstInput);
//                        editor.putStringSet("page_button_key", new HashSet<String>());
//                        editor.apply(); // editor.commit();

                        // after inputting submitting valid password enter main activity
                        Intent intent = new Intent(getApplicationContext(), MainActivity.class);
                        startActivity(intent);
                        finish();
                    }
                    else {
                        Toast.makeText(CreatePasswordActivity.this,"Password Does Not Match!", Toast.LENGTH_SHORT).show();
                    }
                }
            }
        });
    }
}
