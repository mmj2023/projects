const puppeteer = require('puppeteer');

(async () => {
  const browser = await puppeteer.launch();
  const page = await browser.newPage();

  // Load your local HTML file (adjust the file URL accordingly)
  await page.goto('file:/home/mdmmj/projects/html/presentation.html', { waitUntil: 'networkidle0' });

  // Generate the PDF
  await page.pdf({ path: 'output.pdf', format: 'A4' });

  await browser.close();
})();
