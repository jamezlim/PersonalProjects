package com.jameskim.passwordmanager;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

public class DatabaseHelper extends SQLiteOpenHelper {
    // variables
    public static final String DATABASE_NAME = "accounts.db";
    public static final String TABLE_NAME = "accounts";
    public static final String COL0 = "ID";
    public static final String COL1 = "page";
    public static final String COL2 = "account";
    public static final String COL3 = "password";

    //constructor
    public DatabaseHelper(Context context) {
        super(context, DATABASE_NAME, null, 1);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        String createTable = "CREATE TABLE " + TABLE_NAME + "(ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                + "page TEXT, account TEXT, password TEXT)";
        db.execSQL(createTable);
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        db.execSQL("DROP TABLE IF EXISTS " + TABLE_NAME);
    }

    public boolean addAccount(String page, String account, String password) {
        SQLiteDatabase db = this.getWritableDatabase();
        ContentValues record = new ContentValues();
        record.put(COL1, page);
        record.put(COL2, account);
        record.put(COL3, password);

        long result = db.insert(TABLE_NAME, null, record);

        if (result == -1) return false;
        else return true;
    }

    public Cursor getListContents() {
        SQLiteDatabase db = this.getWritableDatabase();
        Cursor data = db.rawQuery("SElECT * FROM " + TABLE_NAME + " ORDER BY page", null);
        return data;
    }
}