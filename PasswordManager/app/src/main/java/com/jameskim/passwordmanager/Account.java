package com.jameskim.passwordmanager;

public class Account {
    private String page;
    private String account;
    private String password;

    public Account(String page, String account, String password) {
        this.page = page;
        this.account = account;
        this.password = password;
    }

    public String getPage() {
        return page;
    }

    public void setPage(String page) {
        this.page = page;
    }

    public String getAccount() {
        return account;
    }

    public void setAccount(String account) {
        this.account = account;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }
}
