package com.jameskim.passwordmanager;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import java.util.ArrayList;


public class AccountListAdapter extends ArrayAdapter<Account> {

    Context context;
    int resource; // adapter view layout

    public AccountListAdapter(@NonNull Context context, int resource, @NonNull ArrayList<Account> objects) {
        super(context, resource, objects);
        this.context = context;
        this.resource = resource;
    }

    @NonNull
    @Override
    // get the view and attach to the listView
    public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {

        String page = getItem(position).getPage();
        String account = getItem(position).getAccount();
        String password = getItem(position).getPassword();

        LayoutInflater inflater = LayoutInflater.from(context);
        convertView = inflater.inflate(resource, parent,false);

        TextView adapterViewPage = (TextView) convertView.findViewById(R.id.adapterViewPage);
        TextView adapterViewAccount = (TextView) convertView.findViewById(R.id.adapterViewAccount);
        TextView adapterViewPassword = (TextView) convertView.findViewById(R.id.adapterViewPassword);

        adapterViewPage.setText(page);
        adapterViewAccount.setText(account);
        adapterViewPassword.setText(password);

        return convertView;
    }
}
