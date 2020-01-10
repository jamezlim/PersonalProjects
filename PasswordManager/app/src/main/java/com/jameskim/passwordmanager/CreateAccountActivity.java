package com.jameskim.passwordmanager;

import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.util.Set;

public class CreateAccountActivity extends AppCompatActivity {

    DatabaseHelper db;
    EditText createAccountPageEditText, createAccountAccountEditText, createAccountPasswordEditText;
    Button createAccountCreateBtn;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_create_account);

        db = new DatabaseHelper(this);

        createAccountPageEditText = (EditText) findViewById(R.id.createAccountPageEditText);
        createAccountAccountEditText = (EditText) findViewById(R.id.createAccountAccountEditText);
        createAccountPasswordEditText = (EditText) findViewById(R.id.createAccountPasswordEditText);

        createAccountCreateBtn = (Button) findViewById(R.id.createAccountCreateBtn);
        createAccountCreateBtn.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                String page = createAccountPageEditText.getText().toString();
                String account = createAccountAccountEditText.getText().toString();
                String password = createAccountPasswordEditText.getText().toString();

                if ( page.length() * account.length() * password.length() == 0){
                    Toast.makeText(CreateAccountActivity.this, "Fill in all entries yo :)", Toast.LENGTH_SHORT).show();
                }
                else{
                    addAccount(page,account,password);
                    // null edit text
                    createAccountAccountEditText.setText("");
                    createAccountPasswordEditText.setText("");
                }

                /*
                  "page_button_key" is a SharedPreferences. It is a key that stores Set of Strings.
                  NOTE: "page_button_key" has empty Set of Strings as a default value (created in "CreatePasswordActivity".
                  So empty indicates that we have no page buttons to restore.
                  ex) Create 4 page title buttons; Google, Amazon, NCR, Facebook (added in given order)
                  "page_button_key" -> Set: {"Google", "Amazon", "NCR", "Facebook"}
                  We can then restore buttons by calling key-value pairs via iteration when entering to
                  MainActivity screen.
                 */

                // load data from SharedPreferences
                SharedPreferences setting = PreferenceManager.getDefaultSharedPreferences(CreateAccountActivity.this);
                Set<String> pageSet = setting.getStringSet("page_button_key", null);

                // update String set
                pageSet.add(page);

                /*
                  update SharedPreference : Note that for String Set in SharedPreferences needs extra care.
                        Modifying returned value from getStringSet() method leads to inconsistency in SharedPreferences
                        and therefore data is no more guaranteed to be stored safely.
                        Possible solution is -> modify the StringSet -> remove key-value pair in the SharedPreferences
                                ->  then putStringSet(StringSetModified).apply()
                 */

                SharedPreferences settings = PreferenceManager.getDefaultSharedPreferences(CreateAccountActivity.this);
                SharedPreferences.Editor editor = settings.edit();
                editor.remove("page_button_key").apply();
                editor.putStringSet("page_button_key", pageSet).apply();

                finish();

            }



        }); // end of setOnClickListener
    }// end of onCreate

    public void addAccount(String col1, String col2, String col3){
        boolean result = db.addAccount(col1,col2,col3);

        if(result) Toast.makeText(this, "Account Successfully Created", Toast.LENGTH_SHORT).show();
        else Toast.makeText(this, "Went Wrong :(", Toast.LENGTH_SHORT).show();

    }
}
