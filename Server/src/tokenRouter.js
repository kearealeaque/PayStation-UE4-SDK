const express = require('express');
const makeRequest = require('request');

module.exports = class TokenRouter {
    constructor(globals) {
        this.globals = globals;

        this.router = express.Router();
        this._setupRouter();
    }

    getRouter() {
        return this.router;
    }

    _setupRouter() {
        this.router.post('/', (req, res, next) => {
            let url = 'https://' 
            + this.globals.merchantId + ':' + this.globals.apiKey 
            + '@api.xsolla.com/merchant/v2/merchants/' 
            + this.globals.merchantId 
            + '/token';

            // load token json 
            let tokenJson = require('./token.json');
            tokenJson.settings.project_id = this.globals.projectId;
            tokenJson.user.id.value = req.raw.body;

            let options = {
                url: url,
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                    'Content-Length': tokenJson.length
                },
                json: tokenJson
            };

            makeRequest(options, (err, tokenRes, body) => {
                if (err) throw err;

                if (tokenRes && body.token) {
                    //this.globals.userIdList.add( JSON.parse(req.rawBody).user.id.value );

                    res.statusCode = this.globals.successStatusCode;
                    res.end(body.token);
                } else {
                    res.statusCode = this.globals.errorStatusCode;
                    res.end();
                }
            });
        });
    }
};